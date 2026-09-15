#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

typedef struct {

	SDL_Texture *texture;
	int w, h;

} SpriteInternal;

static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

static SpriteInternal sprites[MAX_SPRITES];
static int sprite_count;

static SDL_Texture *font;

static uint8_t bg_r, bg_g, bg_b;

void set_background(uint8_t r, uint8_t g, uint8_t b) {

	bg_r = r;
	bg_g = g;
	bg_b = b;
}

int load_sprite(const char *string) {

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

void draw_text(const char *string, int x, int y, double scale) {

	float fx = x;
	float fy = y;
	float w = FONT_W * scale;
	float h = FONT_H * scale;

	while (*string != '\0') {

		int sx = 31;
		int sy = 0;

		if (*string >= 'A' && *string <= 'Z') {

			sx = 1 + *string - 'A';
			sy = 1;

		} else if (*string >= '0' && *string <= '9') {

			sx = *string - '0' + 16;
			sy = 0;

		} else if (*string == ' ') {

			fx += w;
			string++;
			
			continue;
		}

		SDL_Rect src_rect = { sx * FONT_W, sy * FONT_H, FONT_W, FONT_H };
		SDL_Rect dest_rect = { (int) fx, (int) fy, w, h };

		SDL_RenderCopyEx(renderer, font, &src_rect, &dest_rect, 0.0, NULL, SDL_FLIP_NONE);

		fx += w;
		string++;
	}
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