#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#define MAX_SPRITES 32

#define SCREEN_W 600
#define SCREEN_H 400

#define FONT_W 19
#define FONT_H 37

void set_background(uint8_t r, uint8_t g, uint8_t b);
int load_sprite(const char *string);
void draw_sprite(int sprite, int x, int y, double a, double scale);
void draw_text(const char *string, int x, int y, double scale);

void game_init();
void game_update();

#endif