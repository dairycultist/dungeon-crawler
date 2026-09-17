#include "window.h"
#include <math.h>
#include <stdio.h>

typedef struct {

	int sprite; // if -1, the occupant doesn't exist

} Occupant;

typedef struct {

	int is_open; // if false, is a wall
	Occupant occupant;

} Room;

static double animt;

static int spr_bg;
static int spr_left_wall, spr_left_corner, spr_front_wall, spr_right_corner, spr_right_wall;
static int spr_slime;

static int btn_navleft, btn_navforward, btn_navright;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

static Room map[5][5];
static int player_x = 2, player_y = 2, player_dir = NORTH;

void game_init() {

	spr_bg = load_sprite("./res/bg.png");

	spr_left_wall    = load_sprite("./res/left_wall.png");
	spr_left_corner  = load_sprite("./res/left_corner.png");
	spr_front_wall   = load_sprite("./res/front_wall.png");
	spr_right_corner = load_sprite("./res/right_corner.png");
	spr_right_wall   = load_sprite("./res/right_wall.png");

	spr_slime = load_sprite("./res/slime.png");

	btn_navleft    = register_button(1, 256, 79, 291);
	btn_navforward = register_button(80, 256, 157, 291);
	btn_navright   = register_button(158, 256, 236, 291);

	set_text_scale(0.45);

	map[1][1] = (Room){1, -1};
	map[1][2] = (Room){1, spr_slime};
	map[2][2] = (Room){1, -1};
	map[2][3] = (Room){1, -1};
	map[3][1] = (Room){1, -1};
	map[3][2] = (Room){1, -1};
	map[3][3] = (Room){1, -1};
}

void game_update() {

	draw_sprite(spr_bg, 300, 200, 0.0, 0.0, 0.0, 1.0);

	Room *ahead, *left, *right, *ahead_left, *ahead_right;

	switch (player_dir) {
		case NORTH:
			ahead       = &map[player_y - 1][player_x];
			left        = &map[player_y][player_x - 1];
			right       = &map[player_y][player_x + 1];
			ahead_left  = &map[player_y - 1][player_x - 1];
			ahead_right = &map[player_y - 1][player_x + 1];
			break;
		case EAST:
			ahead       = &map[player_y][player_x + 1];
			left        = &map[player_y - 1][player_x];
			right       = &map[player_y + 1][player_x];
			ahead_left  = &map[player_y - 1][player_x + 1];
			ahead_right = &map[player_y + 1][player_x + 1];
			break;
		case SOUTH:
			ahead       = &map[player_y + 1][player_x];
			left        = &map[player_y][player_x + 1];
			right       = &map[player_y][player_x - 1];
			ahead_left  = &map[player_y + 1][player_x + 1];
			ahead_right = &map[player_y + 1][player_x - 1];
			break;
		case WEST:
			ahead       = &map[player_y][player_x - 1];
			left        = &map[player_y + 1][player_x];
			right       = &map[player_y - 1][player_x];
			ahead_left  = &map[player_y + 1][player_x - 1];
			ahead_right = &map[player_y - 1][player_x - 1];
			break;
	}

	if (!ahead_left->is_open)  draw_sprite(spr_left_corner,  7, 6, -1.0, -1.0, 0.0, 1.0);
	if (!ahead_right->is_open) draw_sprite(spr_right_corner, 7, 6, -1.0, -1.0, 0.0, 1.0);
	if (!left->is_open)        draw_sprite(spr_left_wall,    7, 6, -1.0, -1.0, 0.0, 1.0);
	if (!right->is_open)       draw_sprite(spr_right_wall,   7, 6, -1.0, -1.0, 0.0, 1.0);
	if (!ahead->is_open)       draw_sprite(spr_front_wall,   7, 6, -1.0, -1.0, 0.0, 1.0);

	if (map[player_y][player_x].occupant.sprite != -1) {

		draw_sprite(map[player_y][player_x].occupant.sprite, 118, 230, 0.0, 0.8, 0.0, 1.0 + sin(animt * 5.0) * 0.02);
	}

	// info box
	set_text_carriage(250, 270, SCREEN_W - 7);

	// draw occupant warning text
	if (ahead->is_open) {

		if (ahead->occupant.sprite == spr_slime) {

			set_text_color(255, 0, 0);
			draw_text("You hear the squelching of slime ahead of you...");
		}
	}

	// player stats
	set_text_carriage(445, 208, SCREEN_W);
	set_text_color(255, 255, 255);
	draw_text("Hero lvl 3\nstats 40000");

	animt += M_PI / 100.0;
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
					if (map[player_y - 1][player_x].is_open)
						player_y--;
					break;
				case EAST:
					if (map[player_y][player_x + 1].is_open)
						player_x++;
					break;
				case SOUTH:
					if (map[player_y + 1][player_x].is_open)
						player_y++;
					break;
				case WEST:
					if (map[player_y][player_x - 1].is_open)
						player_x--;
					break;
			}
		}
	}
}