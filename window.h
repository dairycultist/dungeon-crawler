#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

#define MAX_SPRITES 32

// screen size
#define WIDTH 600
#define HEIGHT 400

void set_background(uint8_t r, uint8_t g, uint8_t b);
int load_sprite(const char *string);
void draw_sprite(int sprite, int x, int y, double a);
void draw_text(const char *string, int x, int y);

void game_init();
void game_update();

#endif