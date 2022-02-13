#include <SDL_render.h>
#include <stdio.h>

#include "state.h"
#include "log.h"
#include "map.h"

#include "render.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GAME_WIDTH 20.0
#define GAME_HEIGHT 15.0
#define BLOCK_SIZE SCREEN_WIDTH / GAME_WIDTH

#define PLAYER_RATIO_X 1.0
#define PLAYER_RATIO_Y 1.5
#define PLAYER_WIDTH (int) (PLAYER_RATIO_X * (double) BLOCK_SIZE)
#define PLAYER_HEIGHT (int) (PLAYER_RATIO_Y * (double) BLOCK_SIZE)

#define BULLET_RATIO_X 0.25
#define BULLET_RATIO_Y 0.25
#define BULLET_WIDTH (int) (BULLET_RATIO_X * (double) BLOCK_SIZE)
#define BULLET_HEIGHT (int) (BULLET_RATIO_Y * (double) BLOCK_SIZE)

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

	SDL_SetRenderDrawColor(renderer, 0xDD, 0x44, 0x00, 0xFF);
	for (int i = 0; i < (int) state->player_count; i++) {
		struct Player* player = &state->players[i];

		rect = (SDL_Rect) {
			.x = (player->entity.pos_x / GAME_WIDTH) * SCREEN_WIDTH,
			.y = (((GAME_HEIGHT - player->entity.pos_y) / GAME_HEIGHT) * SCREEN_HEIGHT) - PLAYER_HEIGHT,
			.w = PLAYER_WIDTH,
			.h = PLAYER_HEIGHT,
		};

		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 0x25, 0x25, 0x25, 0xFF);
	for (int i = 0; i < (int) state->bullet_count; i++) {
		struct Bullet* bullet = &state->bullets[i];

		rect = (SDL_Rect) {
			.x = (bullet->entity.pos_x / GAME_WIDTH) * SCREEN_WIDTH,
			.y = (((GAME_HEIGHT - bullet->entity.pos_y) / GAME_HEIGHT) * SCREEN_HEIGHT) - BULLET_HEIGHT,
			.w = BULLET_WIDTH,
			.h = BULLET_HEIGHT,
		};

		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_RenderPresent(renderer);
}
