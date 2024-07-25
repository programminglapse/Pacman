#ifndef GLOBALS__H
#define GLOBALS__H

const static int GRID_SIZE = 1120;
const static int GRID_WIDTH = 28;
const static int GRID_HEIGHT = 28;
const static int TILE_SIZE = 40;
const static int BORDER_TILE_SIZE = 30;
const static int INTERIOR_TILE_SIZE = 25;
const static int PLAYER_SIZE = 35;
const static int PLAYER_FRAMES_PER_SPRITE = 400;
const static float PLAYER_VELOCITY = 3.0f;
const static int COIN_SIZE = 10;
const static int POINTS_PER_COIN = 10;
const static int GHOST_SIZE = 35;
const static int GHOST1_INIT_X = 480;
const static int GHOST1_INIT_Y = 530;
const static int GHOST2_INIT_X = 520;
const static int GHOST2_INIT_Y = 530;
const static int GHOST3_INIT_X = 560;
const static int GHOST3_INIT_Y = 530;
const static int GHOST4_INIT_X = 600;
const static int GHOST4_INIT_Y = 530;
const static float GHOST_VELOCITY = 2.0f;
const static float GHOST_EATABLE_VELOCITY = 1.5f;
const static int GHOST_EAT_POINTS = 100;

struct pos
{
	float x;
	float y;
};

enum EDirection
{
	NONE = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

using tile = std::pair<int,int>;

#endif // GLOBALS__H
