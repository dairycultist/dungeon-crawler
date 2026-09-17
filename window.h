#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#define MAX_SPRITES 32
#define MAX_BUTTONS 8

#define SCREEN_W 600
#define SCREEN_H 400

#define FONT_W 19
#define FONT_H 37
#define LINE_H 28

#define BUTTON_HOVER 0
#define BUTTON_CLICK 1

int register_button(int x1, int y1, int x2, int y2);

void set_background(uint8_t r, uint8_t g, uint8_t b);

int load_sprite(const char *string);
void draw_sprite(int sprite, int x, int y, double pivot_x, double pivot_y, double a, double scale); // pivot is in range [-1.0, 1.0]

// all three set functions must be called at least once, with set_text_carriage
// needing to be called at least every frame (to reset the carriage back to the
// start of the text region)
void set_text_color(uint8_t r, uint8_t g, uint8_t b);
void set_text_carriage(int start_x, int start_y, int end_x);
void set_text_scale(double scale);
void draw_text(const char *string);

void game_init();
void game_update();
void on_button_event(int button, int state);

#endif