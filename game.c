#include "window.h"
#include <math.h>

static double b;

void game_init() {

	load_sprite("./res/font.png");
}

void game_update() {

	draw_text(" !\"#$%&'()*+,-./0123456789:;<=>?", 0, 0, 0.9 + sin(b) * 0.1);
	draw_text("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", 0, 40, 0.9 + sin(b) * 0.1);
	draw_text("`abcdefghijklmnopqrstuvwxyz{|}~熏肉", 0, 80, 0.9 + sin(b) * 0.1);

	b += 0.1;
}