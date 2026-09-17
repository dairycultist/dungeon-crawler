#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

typedef struct {

	SDL_Texture *texture;
	int w, h;

} SpriteInternal;

typedef struct {

	int x1, y1, x2, y2;

} ButtonInternal;

static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

static SpriteInternal sprites[MAX_SPRITES];
static int sprite_count;

static ButtonInternal buttons[MAX_BUTTONS];
static int button_count;
static int button_down_on = -1;

static SDL_Texture *font;
static int text_start_x;
static int text_end_x;
static float text_x;
static float text_y;
static float text_font_w = FONT_W;
static float text_font_h = FONT_H;
static float text_line_h = LINE_H;

static uint8_t bg_r, bg_g, bg_b;

void set_background(uint8_t r, uint8_t g, uint8_t b) {

	bg_r = r;
	bg_g = g;
	bg_b = b;
}

int load_sprite(const char *string) {

	if (sprite_count == MAX_SPRITES) {

		printf("Could not load sprite %s because ran out of space (increase the MAX_SPRITES macro!)", string);
		exit(1);
	}

	sprites[sprite_count].texture = IMG_LoadTexture(renderer, string);

	SDL_QueryTexture(sprites[sprite_count].texture, NULL, NULL, &sprites[sprite_count].w, &sprites[sprite_count].h);

	sprite_count++;
}

void draw_sprite(int sprite, int x, int y, double a, double scale) {

	int w = (int) (sprites[sprite].w * scale);
	int h = (int) (sprites[sprite].h * scale);

	SDL_Rect dest_rect = { x - w / 2, y - h / 2, w, h };

	SDL_RenderCopyEx(renderer, sprites[sprite].texture, NULL, &dest_rect, a, NULL, SDL_FLIP_NONE);
}

void set_text_color(Uint8 r, Uint8 g, Uint8 b) {

	SDL_SetTextureColorMod(font, r, g, b);
}

void set_text_carriage(int start_x, int start_y, int end_x) {

	text_start_x = start_x;
	text_end_x = end_x;

	text_x = start_x;
	text_y = start_y;
}

void set_text_scale(double scale) {

	text_font_w = FONT_W * scale;
	text_font_h = FONT_H * scale;
	text_line_h = LINE_H * scale;
}

void draw_text(const char *string) {

	while (*string != '\0') {

		int i;

		if (*string >= ' ' && *string <= '~') {

			i = *string - ' ';
		
		} else if (*string == '\n') {

			text_x = text_start_x;
			text_y += text_line_h;
			string++;
			continue;

		} else {

			i = 31; // '?'
		}

		// check if this next character will go past end_x; if so, wrap
		if (text_x + text_font_w >= text_end_x) {

			text_x = text_start_x;
			text_y += text_line_h;
		}

		SDL_Rect src_rect = { (i % 32) * FONT_W, (i / 32) * FONT_H, FONT_W, FONT_H };
		SDL_Rect dest_rect = { (int) text_x, (int) text_y, (int) text_font_w, (int) text_font_h };

		SDL_RenderCopyEx(renderer, font, &src_rect, &dest_rect, 0.0, NULL, SDL_FLIP_NONE);

		text_x += text_font_w;
		string++;
	}
}

int register_button(int x1, int y1, int x2, int y2) {

	if (button_count == MAX_BUTTONS) {

		printf("Could not load button because ran out of space (increase the MAX_BUTTONS macro!)");
		exit(1);
	}

	buttons[button_count].x1 = x1;
	buttons[button_count].y1 = y1;
	buttons[button_count].x2 = x2;
	buttons[button_count].y2 = y2;

	button_count++;
}

int main(void) {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W * 2, SCREEN_H * 2, SDL_WINDOW_RESIZABLE);

	if (!window) {
		fprintf(stderr, "Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);

	if (!screen_buffer) {
		fprintf(stderr, "Error creating screen buffer:\n%s\n", SDL_GetError());
		return 1;
	}

	font = IMG_LoadTexture(renderer, "./res/font.png");

	if (!font) {
		fprintf(stderr, "Could not read font.png\n");
		return 1;
	}

	// init
	game_init();

	// main loop
	SDL_Event event = {0};
	SDL_Rect letterbox = { 0, 0, SCREEN_W * 2, SCREEN_H * 2 };

	while (1) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				goto destroy;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				#define MIN(a, b) ((a) > (b) ? (b) : (a))
				#define ASPECT_RATIO (SCREEN_W / (float) SCREEN_H)

				// dynamically change letterbox based on screen resize
				letterbox.w = MIN(event.window.data1, event.window.data2 * ASPECT_RATIO);
				letterbox.h = MIN(event.window.data2, event.window.data1 / ASPECT_RATIO);

				letterbox.x = (event.window.data1 - letterbox.w) / 2;
				letterbox.y = (event.window.data2 - letterbox.h) / 2;

			} else if (event.type == SDL_MOUSEBUTTONDOWN) {

				event.button.x -= letterbox.x;
				event.button.y -= letterbox.y;
				event.button.x = event.button.x * SCREEN_W / letterbox.w;
				event.button.y = event.button.y * SCREEN_H / letterbox.h;
				
				for (int i = 0; i < button_count; i++) {

					if (event.button.x >= buttons[i].x1 &&
						event.button.y >= buttons[i].y1 &&
						event.button.x <  buttons[i].x2 &&
						event.button.y <  buttons[i].y2
					) {

						button_down_on = i;
						continue;
					}
				}

			} else if (event.type == SDL_MOUSEBUTTONUP) {

				event.button.x -= letterbox.x;
				event.button.y -= letterbox.y;
				event.button.x = event.button.x * SCREEN_W / letterbox.w;
				event.button.y = event.button.y * SCREEN_H / letterbox.h;

				if (button_down_on != -1) {

					if (event.button.x >= buttons[button_down_on].x1 &&
						event.button.y >= buttons[button_down_on].y1 &&
						event.button.x <  buttons[button_down_on].x2 &&
						event.button.y <  buttons[button_down_on].y2
					) {

						on_button_event(button_down_on, BUTTON_CLICK);
					}

					button_down_on = 0;
				}
			}
		}

		// button hovering
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		mouse_x -= letterbox.x;
		mouse_y -= letterbox.y;
		mouse_x = mouse_x * SCREEN_W / letterbox.w;
		mouse_y = mouse_y * SCREEN_H / letterbox.h;

		for (int i = 0; i < button_count; i++) {

			if (mouse_x >= buttons[i].x1 &&
				mouse_y >= buttons[i].y1 &&
				mouse_x <  buttons[i].x2 &&
				mouse_y <  buttons[i].y2
			) {

				on_button_event(i, BUTTON_HOVER);
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); 			// clear window to grey
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 				// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 255); 	// clear screen_buffer to black
		SDL_RenderClear(renderer);

		game_update();

		SDL_SetRenderTarget(renderer, NULL); 						// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 	// render screen_buffer
		SDL_RenderPresent(renderer); 								// present rendered content to screen

		SDL_Delay(1000 / 60);
	}

	// clean up
	destroy:

	SDL_DestroyTexture(font);

	for (int i = 0; i < sprite_count; i++)
		SDL_DestroyTexture(sprites[i].texture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}