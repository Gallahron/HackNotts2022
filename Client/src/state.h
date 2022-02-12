#ifndef __STATE_H__
#define __STATE_H__

#include <stdbool.h>
#include <pthread.h>

#define MAX_PLAYERS 8
#define MAX_BULLETS 256

typedef int Player;
typedef int Bullet;

struct State {
	bool playing;
	double pos_x;
	double pos_y;
	/* new state */
	Player players[MAX_PLAYERS];
	Bullet bullets[MAX_BULLETS];
};

struct StateManager {
	pthread_mutex_t mutex; /* do not swap front & back unless controlling this */
	struct State* front;
	struct State* back;
};

struct StateManager* state_mgr_init();
void state_mgr_destroy(struct StateManager* state_mgr);

#endif
