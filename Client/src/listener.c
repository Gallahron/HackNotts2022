#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "state.h"

#include "listener.h"

struct ListenerState* listener_init(struct StateManager* state_mgr);
void listener_destroy(struct ListenerState* listener);
void* listener(void* args);

struct ListenerState {
	pthread_t thread_id;
	struct ListenerArgs {
		struct StateManager* state_mgr;
	} args;
};

struct ListenerState* listener_init(struct StateManager* state_mgr)
{
	struct ListenerState* listener_state = NULL;

	if (!(listener_state = malloc(sizeof(*listener_state))))
		goto cleanup;

	listener_state->args.state_mgr = state_mgr;

	if (pthread_create(&listener_state->thread_id, NULL, &listener, &listener_state->args) != 0)
		goto cleanup;

	return listener_state;

cleanup:
	if (listener_state)
		free(listener_state);

	return NULL;
}

void listener_destroy(struct ListenerState* listener)
{
	pthread_kill(listener->thread_id, SIGKILL);
	pthread_join(listener->thread_id, NULL);

	free(listener);
}

void* listener(void* listener_args)
{
	struct ListenerArgs args = *(struct ListenerArgs*) listener_args;
	struct State* state;

	while (true) {
		usleep(16 * 1000);

		/* update back state (TODO: get from server) */

		state = args.state_mgr->back;

		state->pos_x = state->pos_x + 0.25;
		if (state->pos_x > 20.0)
			state->pos_x = 0 - 1 + (20.0 - state->pos_x);

		/* swap states */

		pthread_mutex_lock(&args.state_mgr->mutex);

		state = args.state_mgr->back;
		args.state_mgr->back = args.state_mgr->front;
		args.state_mgr->front = state;

		pthread_mutex_unlock(&args.state_mgr->mutex);
	}

	return NULL;
}
