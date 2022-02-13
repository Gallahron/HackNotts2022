#include <stdbool.h>
#include <sysexits.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>

#include <SDL.h>
#include <SDL_net.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_timer.h>
#include <SDL_image.h>

#include "state.h"
#include "render.h"
#include "listener.h"
#include "input.h"

#include "game.h"

enum SDL_USEREVENTS {
	TICK = 1,
};

int game(struct in_addr server_addr, bool verbose);
Uint32 tick_event(Uint32 interval, void *param);

int game(struct in_addr server_addr, bool verbose)
{
	int exit_code = EX_OK;
	struct StateManager* state_mgr = NULL;
	struct InputManager* input_mgr = NULL;
	struct ListenerState* listener = NULL;
	struct Textures* textures = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_TimerID timer_tick = 0;
	SDL_Event event;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_TIMER);
	SDLNet_Init();

	if (!(window = SDL_CreateWindow("My SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN))) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	if (!(state_mgr = state_mgr_init())) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	if (!(input_mgr = input_mgr_init())) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	if (!(listener = listener_init(state_mgr, input_mgr, server_addr))) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	if (!(textures = tex_init(renderer))) {
		exit_code = EX_SOFTWARE;
		goto cleanup;
	}

	SDL_SetWindowMinimumSize(window, 640, 480);
	SDL_SetWindowMaximumSize(window, 640, 480);

	SDL_RegisterEvents(1);
	timer_tick = SDL_AddTimer(16, tick_event, NULL);

	loadFont();

	bool quit = false;
	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
			case SDL_USEREVENT:
				if (event.user.code == TICK)
				{
					pthread_mutex_lock(&state_mgr->mutex);
					
					render(state_mgr->front, textures, renderer);

					pthread_mutex_unlock(&state_mgr->mutex);
				}
				break;
			case SDL_KEYDOWN:
				keypress(input_mgr, event.key.keysym.sym, true);
				break;
			case SDL_KEYUP:
				keypress(input_mgr, event.key.keysym.sym, false);
				break;
			case SDL_QUIT:
				quit = true;
				break;
		}
	}

cleanup:
	if (textures)
		tex_destroy(textures);

	if (timer_tick != 0)
		SDL_RemoveTimer(timer_tick);

	if (window)
		SDL_DestroyWindow(window);

	if (renderer)
		SDL_DestroyRenderer(renderer);

	if (listener)
		listener_destroy(listener);

	if (state_mgr)
		state_mgr_destroy(state_mgr);

	if (input_mgr)
		input_mgr_destroy(input_mgr);

	SDL_Quit();

	return exit_code;
}

Uint32 tick_event(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = TICK;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);

    return interval;
}
