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
bool inputs_compare(struct Inputs* one, struct Inputs* two);

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
	case SDLK_LEFT: input_mgr->inputs.left = keydown; break;
	case SDLK_RIGHT: input_mgr->inputs.right = keydown; break;
	case SDLK_UP: input_mgr->inputs.jump = keydown; break;
	case SDLK_SPACE: input_mgr->inputs.shoot = keydown; break;
	}
	pthread_mutex_unlock(&input_mgr->mutex);
}

bool inputs_compare(struct Inputs* one, struct Inputs* two)
{
	if (one->left == two->left)
		if (one->right == two->right)
			if (one->jump == two->jump)
				if (one->shoot == two->shoot)
					return true;

	return false;
}
