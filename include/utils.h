/*
// utils: Utility functions
*/

#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
SDL_Rect set_rect(int x, int y, int w, int h);

#endif /* UTILS_H */
