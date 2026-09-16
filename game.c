#include "window.h"
#include <math.h>

static double b;

void game_init() {

	load_sprite("./res/bg.png");
	set_text_color(255, 0, 0);
}

void game_update() {

	draw_sprite(0, 300, 200, 0.0, 1.0);

	draw_text("You sense a dangerous presence ahead of\nyou...", 250, 270, 0.45);

	b += 0.1;
}