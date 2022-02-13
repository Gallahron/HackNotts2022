#ifndef __STATE_H__
#define __STATE_H__

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

#include "map.h"

#define MAX_PLAYERS 16
#define MAX_BULLETS 256

enum BulletType {
	BL_STANDARD = 0,
	BL_BOMB,
};

struct Entity {
	double pos_x, pos_y;
	double speed_x, speed_y;
};

struct Player {
	struct Entity entity;
	unsigned int player_number;
	unsigned int lives;
};

struct Bullet {
	struct Entity entity;
	enum BulletType type;
	unsigned int time_left;
};

struct State {
	bool playing;
	double pos_x;
	double pos_y;
	/* new state */
	struct MapData* map_data;
	struct Player players[MAX_PLAYERS];
	struct Bullet bullets[MAX_BULLETS];
	size_t player_count, bullet_count;
};

struct StateManager {
	pthread_mutex_t mutex; /* do not swap front & back unless controlling this */
	struct State* front;
	struct State* back;
};

struct StateManager* state_mgr_init();
void state_mgr_destroy(struct StateManager* state_mgr);

#endif
