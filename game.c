#include "window.h"
#include <math.h>
#include <stdio.h>

static double b;

void game_init() {

	load_sprite("./res/bg.png");
	register_button(0, 0, 200, 200);
}

void game_update() {

	draw_sprite(0, 300, 200, 0.0, 1.0);

	set_text_color(255, 0, 0);
	draw_text("You sense a dangerous presence ahead of\nyou...", 250, 270, 0.45);

	set_text_color(255, 255, 255);
	draw_text("Hero lvl 3\nstats 40000", 445, 208, 0.45);

	b += 0.1;
}

void on_button_event(int button, int state) {

	printf("button #%d: %s\n", button, state == BUTTON_HOVER ? "hover" : "click");
}