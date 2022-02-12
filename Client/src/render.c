#include <SDL_render.h>

#include "state.h"

#include "render.h"

void render(struct State* state, SDL_Renderer* renderer);

void render(struct State* state, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	SDL_Rect rect = {
		.x = ((state->pos_x / 20.0) * 640),
		.y = 224,
		.w = 32,
		.h = 32,
	};

	SDL_RenderDrawRect(renderer, &rect);

	SDL_RenderPresent(renderer);
}
