#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL_render.h>
#include <SDL_image.h>

#include "state.h"

struct Textures {
	SDL_Texture* tex_clouds;
	SDL_Texture* tex_ground;
	SDL_Texture* tex_platform;
	SDL_Texture* tex_player_blue_left;
	SDL_Texture* tex_player_blue_right;
	SDL_Texture* tex_player_red_left;
	SDL_Texture* tex_player_red_right;
};

struct Textures* tex_init(SDL_Renderer* renderer);
void tex_destroy(struct Textures* textures);
void render(struct State* state, struct Textures* textures, SDL_Renderer* renderer);
void loadFont();void loadFont();

#endif
