#include "window.h"
#include <math.h>

static double b;

void game_init() {

	load_sprite("./res/font.png");
}

void game_update() {

	draw_text("APPLE 123", 0, 0, 1.0 + sin(b));

	b += 0.01;
}