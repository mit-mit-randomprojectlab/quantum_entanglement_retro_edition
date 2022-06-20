/*
// title.h: contains objects for titlescreen etc.
*/

#ifndef TITLE_H
#define TITLE_H

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "consts.h"
#include "utils.h"

#include "resources.h"

void InitTitle(void);

// Event handling
void OnEvent_Title(SDL_Event *event, SceneRequest *scenerequest);

// Update functions
void OnUpdate_Title(SceneRequest *scenerequest);

// Drawing Functions
void OnDraw_Title(SDL_Renderer *renderer);

#endif /* TITLE_H */
