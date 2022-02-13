#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "map.h"

#include "state.h"

struct StateManager* state_mgr_init();
void state_mgr_destroy(struct StateManager* state_mgr);
struct State* state_init();
void state_destroy(struct State* state);

struct StateManager* state_mgr_init()
{
	struct StateManager* state_mgr;
	bool mutex_init;

	if (!(state_mgr = malloc(sizeof(*state_mgr))))
		goto cleanup;

	if (!(state_mgr->front = state_init()))
		goto cleanup;

	if (!(state_mgr->back = state_init()))
		goto cleanup;

	if (!(mutex_init = pthread_mutex_init(&state_mgr->mutex, NULL) == 0))
		goto cleanup;

	return state_mgr;

cleanup:
	if (state_mgr && mutex_init)
		pthread_mutex_destroy(&state_mgr->mutex);

	if (state_mgr && state_mgr->back)
		free(state_mgr->back);

	if (state_mgr && state_mgr->front)
		free(state_mgr->front);

	if (state_mgr)
		free(state_mgr);

	return NULL;
}

void state_mgr_destroy(struct StateManager* state_mgr)
{
	if (!state_mgr)
		return;

	pthread_mutex_destroy(&state_mgr->mutex);
	free(state_mgr->front);
	free(state_mgr->back);
	free(state_mgr);
}

struct State* state_init()
{
	struct State* state = malloc(sizeof(*state));

	if (!state)
		return NULL;

	state->playing = true;
	state->pos_x = 0.0;
	state->pos_y = 8.0;

	state->map_data = map_init(MP_STANDARD);

	return state;
}

void state_destroy(struct State* state)
{
	if (state)
		map_destroy(state->map_data);

	if (state)
		free(state);
}
