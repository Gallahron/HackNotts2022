#include <SDL_keycode.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stddef.h>
#include <stdbool.h>

#include <SDL.h>

#include "input.h"

struct InputManager* input_mgr_init();
void input_mgr_destroy(struct InputManager* input_mgr);
void keypress(struct InputManager* input_mgr, SDL_Keycode keycode, bool keydown);

struct InputManager* input_mgr_init()
{
	struct InputManager* input_mgr;

	if (!(input_mgr = calloc(1, sizeof(*input_mgr))))
		goto input_mgr_init_cleanup;

	if (pthread_mutex_init(&input_mgr->mutex, NULL) != 0)
		goto input_mgr_init_cleanup;

	return input_mgr;

input_mgr_init_cleanup:

	if (input_mgr)
		free(input_mgr);

	return NULL;
}

void input_mgr_destroy(struct InputManager* input_mgr)
{
	if (!input_mgr)
		return;

	pthread_mutex_destroy(&input_mgr->mutex);
	free(input_mgr);
}

void keypress(struct InputManager* input_mgr, SDL_Keycode keycode, bool keydown)
{
	pthread_mutex_lock(&input_mgr->mutex);
	switch (keycode) {
	case SDLK_LEFT: input_mgr->left = keydown; break;
	case SDLK_RIGHT: input_mgr->right = keydown; break;
	case SDLK_UP: input_mgr->jump = keydown; break;
	case SDLK_SPACE: input_mgr->shoot = keydown; break;
	}
	pthread_mutex_unlock(&input_mgr->mutex);
}
