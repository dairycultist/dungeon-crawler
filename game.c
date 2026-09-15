#include "window.h"

static double speen;

void game_init() {

	load_sprite("./res/font.png");
}

void game_update() {

	draw_sprite(0, WIDTH / 2, HEIGHT / 2, speen);

	draw_text("APPLE 123", 0, 0);

	speen += 0.1;
}