#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#define MAX_SPRITES 32
#define MAX_BUTTONS 8

#define SCREEN_W 600
#define SCREEN_H 400

#define FONT_W 19
#define FONT_H 37
#define LINE_H 26

#define BUTTON_HOVER 0
#define BUTTON_CLICK 1

int register_button(int x1, int y1, int x2, int y2);

void set_background(uint8_t r, uint8_t g, uint8_t b);

int load_sprite(const char *string);
void draw_sprite(int sprite, int x, int y, double a, double scale);

void set_text_color(uint8_t r, uint8_t g, uint8_t b);
void set_text_carriage(int start_x, int start_y, int end_x);
void set_text_scale(double scale);
void draw_text(const char *string);

void game_init();
void game_update();
void on_button_event(int button, int state);

#endif