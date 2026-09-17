#include "window.h"
#include <math.h>
#include <stdio.h>

static double b;

static int spr_bg;
static int spr_left_wall, spr_left_corner, spr_front_wall, spr_right_corner, spr_right_wall;

static int btn_navleft, btn_navforward, btn_navright;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

static int map[][5] = {
	{1, 1, 1, 1, 1},
	{1, 0, 0, 1, 1},
	{1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1},
};
static int player_x = 2, player_y = 2, player_dir = NORTH;

void game_init() {

	spr_bg = load_sprite("./res/bg.png");

	spr_left_wall    = load_sprite("./res/left_wall.png");
	spr_left_corner  = load_sprite("./res/left_corner.png");
	spr_front_wall   = load_sprite("./res/front_wall.png");
	spr_right_corner = load_sprite("./res/right_corner.png");
	spr_right_wall   = load_sprite("./res/right_wall.png");

	btn_navleft    = register_button(1, 256, 79, 291);
	btn_navforward = register_button(80, 256, 157, 291);
	btn_navright   = register_button(158, 256, 236, 291);

	set_text_scale(0.45);
}

void game_update() {

	draw_sprite(spr_bg, 300, 200, 0.0, 1.0);

	int ahead, left, right, ahead_left, ahead_right;

	switch (player_dir) {
		case NORTH:
			ahead       = map[player_y - 1][player_x];
			left        = map[player_y][player_x - 1];
			right       = map[player_y][player_x + 1];
			ahead_left  = map[player_y - 1][player_x - 1];
			ahead_right = map[player_y - 1][player_x + 1];
			break;
		case EAST:
			ahead       = map[player_y][player_x + 1];
			left        = map[player_y - 1][player_x];
			right       = map[player_y + 1][player_x];
			ahead_left  = map[player_y - 1][player_x + 1];
			ahead_right = map[player_y + 1][player_x + 1];
			break;
		case SOUTH:
			ahead       = map[player_y + 1][player_x];
			left        = map[player_y][player_x + 1];
			right       = map[player_y][player_x - 1];
			ahead_left  = map[player_y + 1][player_x + 1];
			ahead_right = map[player_y + 1][player_x - 1];
			break;
		case WEST:
			ahead       = map[player_y][player_x - 1];
			left        = map[player_y + 1][player_x];
			right       = map[player_y - 1][player_x];
			ahead_left  = map[player_y + 1][player_x - 1];
			ahead_right = map[player_y - 1][player_x - 1];
			break;
	}

	if (ahead_left)  draw_sprite(spr_left_corner,  118, 129, 0.0, 1.0);
	if (ahead_right) draw_sprite(spr_right_corner, 118, 129, 0.0, 1.0);
	if (left)        draw_sprite(spr_left_wall,    118, 129, 0.0, 1.0);
	if (right)       draw_sprite(spr_right_wall,   118, 129, 0.0, 1.0);
	if (ahead)       draw_sprite(spr_front_wall,   118, 129, 0.0, 1.0);

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

	if (state == BUTTON_CLICK) {
		
		if (button == btn_navleft) {

			if (player_dir == 0)
				player_dir = 3;
			else
				player_dir--;

		} else if (button == btn_navright) {

			if (player_dir == 3)
				player_dir = 0;
			else
				player_dir++;

		} else if (button == btn_navforward) {

			switch (player_dir) {
				case NORTH:
					if (!map[player_y - 1][player_x])
						player_y--;
					break;
				case EAST:
					if (!map[player_y][player_x + 1])
						player_x++;
					break;
				case SOUTH:
					if (!map[player_y + 1][player_x])
						player_y++;
					break;
				case WEST:
					if (!map[player_y][player_x - 1])
						player_x--;
					break;
			}
		}
	}
}