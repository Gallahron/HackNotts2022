#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

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

#define HEART_SIZE 20
#define HEART_SPACING 10

TTF_Font* font;

struct Textures* tex_init(SDL_Renderer* renderer);
void tex_destroy(struct Textures* textures);
void render(struct State* state, struct Textures* textures, SDL_Renderer* renderer);

void loadFont()
{
	TTF_Init();
	font = TTF_OpenFont("./resources/Rowdies-Regular.ttf", 30);
}

struct Textures* tex_init(SDL_Renderer* renderer)
{
	struct Textures* textures = calloc(1, sizeof(*textures));

	textures->tex_clouds = IMG_LoadTexture(renderer, "./resources/clouds_640x480.png");
	textures->tex_ground = IMG_LoadTexture(renderer, "./resources/ground_32x32.png");
	textures->tex_platform = IMG_LoadTexture(renderer, "./resources/platform_32x32.png");
	textures->tex_player_blue_left = IMG_LoadTexture(renderer, "./resources/player_blue_left_32x48.png");
	textures->tex_player_blue_right = IMG_LoadTexture(renderer, "./resources/player_blue_right_32x48.png");
	textures->tex_player_red_left = IMG_LoadTexture(renderer, "./resources/player_red_left_32x48.png");
	textures->tex_player_red_right = IMG_LoadTexture(renderer, "./resources/player_red_right_32x48.png");

	return textures;
}

void tex_destroy(struct Textures* textures)
{
	if (!textures)
		return;

	SDL_DestroyTexture(textures->tex_clouds);
	SDL_DestroyTexture(textures->tex_ground);
	SDL_DestroyTexture(textures->tex_platform);
	SDL_DestroyTexture(textures->tex_player_blue_left);
	SDL_DestroyTexture(textures->tex_player_blue_right);
	SDL_DestroyTexture(textures->tex_player_red_left);
	SDL_DestroyTexture(textures->tex_player_red_right);
	free(textures);
}

void render(struct State* state, struct Textures* textures, SDL_Renderer* renderer)
{
	SDL_Rect rect;

	int cloud_offset = (SDL_GetTicks() / 128) % 640;

	if (cloud_offset >= 640)
		cloud_offset = 0;

	SDL_SetRenderDrawColor(renderer, 0x68, 0x9F, 0xFF, 255);
	SDL_RenderClear(renderer);

	rect = (SDL_Rect) {
		.x = cloud_offset,
		.y = 0,
		.w = 640,
		.h = 480,
	};
	SDL_RenderCopy(renderer, textures->tex_clouds, NULL, &rect);
	rect = (SDL_Rect) {
		.x = cloud_offset + 640,
		.y = 0,
		.w = 640,
		.h = 480,
	};
	SDL_RenderCopy(renderer, textures->tex_clouds, NULL, &rect);
	rect = (SDL_Rect) {
		.x = cloud_offset - 640,
		.y = 0,
		.w = 640,
		.h = 480,
	};
	SDL_RenderCopy(renderer, textures->tex_clouds, NULL, &rect);

	cloud_offset++;

	struct MapData* map_data = state->map_data;
	for (int y = MAP_SIZE_Y - 1; y >= 0; y--) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			enum Block block = map_data->blocks[BLOCK_INDEX(x, y)];

			SDL_Texture* block_texture;

			switch (block) {
			case BK_AIR:      continue;
			case BK_GROUND:   block_texture = textures->tex_ground; break;
			case BK_PLATFORM: block_texture = textures->tex_platform; break;
			}

			if (block == BK_PLATFORM) {
				rect = (SDL_Rect) {
					.x = (x * BLOCK_SIZE) + 12,
					.y = (((MAP_SIZE_Y - y) - 1) * BLOCK_SIZE) + 8,
					.w = BLOCK_SIZE,
					.h = BLOCK_SIZE,
				};

				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x20, 125);
				SDL_RenderFillRect(renderer, &rect);
			}

			rect = (SDL_Rect) {
				.x = x * BLOCK_SIZE,
				.y = ((MAP_SIZE_Y - y) - 1) * BLOCK_SIZE,
				.w = BLOCK_SIZE,
				.h = BLOCK_SIZE,
			};

			SDL_RenderCopy(renderer, block_texture, NULL, &rect);
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

		if ((int) player->player_number == state->player_number_self) {
			if (player->entity.speed_x < 0.0)
				SDL_RenderCopy(renderer, textures->tex_player_blue_left, NULL, &rect);
			else
				SDL_RenderCopy(renderer, textures->tex_player_blue_right, NULL, &rect);
		} else {
			if (player->entity.speed_x < 0.0)
				SDL_RenderCopy(renderer, textures->tex_player_red_left, NULL, &rect);
			else
				SDL_RenderCopy(renderer, textures->tex_player_red_right, NULL, &rect);
		}

		if ((int)player->player_number == state->player_number_self) {
			if (player->lives > 0) {
				for (unsigned int i = 0; i < player->lives; i++) {
					rect = (SDL_Rect) {
						.x = i * (HEART_SIZE + HEART_SPACING) + HEART_SPACING,
						.y = HEART_SPACING,
						.w = HEART_SIZE,
						.h = HEART_SIZE,
					};
					
					SDL_RenderFillRect(renderer, &rect);
				}
			} else {
				if (!font) fprintf(stderr, "ERROR IN FONT LOADING!\n");
				SDL_Color red = {255,0,0,255};
				SDL_Surface* surface_message = TTF_RenderText_Solid(font, "You Lose...", red);
				SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surface_message);
				rect = (SDL_Rect) {
					.x = 30,
					.y = 0,
					.w = 600,
					.h = 300,
				};
				SDL_RenderCopy(renderer, message, NULL, &rect);
				SDL_FreeSurface(surface_message);
				SDL_DestroyTexture(message);
			}
		}
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
