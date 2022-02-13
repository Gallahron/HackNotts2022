#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL_render.h>

#include "state.h"

void loadFont();
void render(struct State* state, SDL_Renderer* renderer);

#endif
