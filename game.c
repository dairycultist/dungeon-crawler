#include "window.h"

static double speen;

void game_init() {

	load_sprite("./res/font.png");
}

void game_update() {

	draw_sprite(0, SCREEN_W / 2, SCREEN_H / 2, speen, 2.3);

	draw_text("APPLE 123", 0, 0);

	speen += 0.1;
}