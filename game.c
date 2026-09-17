#include "window.h"
#include <math.h>
#include <stdio.h>

static double b;

void game_init() {

	load_sprite("./res/bg.png");
	register_button(0, 0, 200, 200);

	set_text_scale(0.45);
}

void game_update() {

	draw_sprite(0, 300, 200, 0.0, 1.0);

	set_text_carriage(250, 270, SCREEN_W - 7);
	set_text_color(255, 0, 0);
	draw_text("You sense a dangerous presence ahead of you...");
	set_text_scale(0.65);
	draw_text(" but are you sure?");
	set_text_scale(0.45);

	set_text_carriage(445, 208, SCREEN_W);
	set_text_color(255, 255, 255);
	draw_text("Hero lvl 3\nstats 40000");

	b += 0.1;
}

void on_button_event(int button, int state) {

	printf("button #%d: %s\n", button, state == BUTTON_HOVER ? "hover" : "click");
}