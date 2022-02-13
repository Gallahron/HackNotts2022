#include <SDL_render.h>
#include <stdio.h>

#include "state.h"
#include "log.h"
#include "map.h"

#include "render.h"

#define BLOCK_SIZE 32

void render(struct State* state, SDL_Renderer* renderer);

void render(struct State* state, SDL_Renderer* renderer)
{
	SDL_Rect rect;

	SDL_SetRenderDrawColor(renderer, 0x51, 0x88, 0xF7, 255);
	SDL_RenderClear(renderer);

	struct MapData* map_data = state->map_data;
	for (int y = MAP_SIZE_Y - 1; y >= 0; y--) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			enum Block block = map_data->blocks[BLOCK_INDEX(x, y)];

			switch (block) {
			case BK_AIR:      continue;
			case BK_GROUND:   SDL_SetRenderDrawColor(renderer, 0x0F, 0x99, 0x44, 0xFF); break;
			case BK_PLATFORM: SDL_SetRenderDrawColor(renderer, 0xFF, 0xD7, 0x00, 0xFF); break;
			}

			rect = (SDL_Rect) {
				.x = x * BLOCK_SIZE,
				.y = ((MAP_SIZE_Y - y) - 1) * BLOCK_SIZE,
				.w = BLOCK_SIZE,
				.h = BLOCK_SIZE,
			};

			SDL_RenderFillRect(renderer, &rect);
		}
	}



	SDL_RenderPresent(renderer);
}
