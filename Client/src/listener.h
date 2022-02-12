#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <pthread.h>

#include "state.h"

struct ListenerState;

struct ListenerState* listener_init(struct StateManager* state_mgr);
void listener_destroy(struct ListenerState* listener);

#endif
