#ifndef __INPUT_H__
#define __INPUT_H__

#include <pthread.h>
#include <stddef.h>
#include <stdbool.h>

#include <SDL.h>

// struct Inputs {
// 	bool left, right, jump, shoot;
// }

struct InputManager {
	pthread_mutex_t mutex;
	bool left, right, jump, shoot;
};

struct InputManager* input_mgr_init();
void input_mgr_destroy(struct InputManager* input_mgr);
void keypress(struct InputManager* input_mgr, SDL_Keycode keycode, bool keydown);

#endif
