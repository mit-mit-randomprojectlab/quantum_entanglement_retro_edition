/*
// resources.h: store reference to resources such as gfx, sfx and music
*/

#ifndef RESOURCES_H
#define RESOURCES_H

#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "consts.h"

typedef struct {
    SDL_Texture *texture;
    int w;
    int h;
} Texture_wh;

typedef struct {
    TTF_Font *font;
    TTF_Font *fontbig;
    Texture_wh title;
    Texture_wh title2;
    Texture_wh start;
    Texture_wh complete;
    Texture_wh failed;
} Text;

typedef struct {
    Mix_Music *music_A;
    Mix_Chunk *sound_step;
    Mix_Chunk *sound_fail;
    Mix_Chunk *sound_complete;
} AudioResources;

typedef struct {
    char name[20];
    int ldata_left[PLAYSPACE_NY][PLAYSPACE_NX];
    int ldata_right[PLAYSPACE_NY][PLAYSPACE_NX];
} Level;

typedef struct {
    int n;
    Level *levels;
    int nlist;
    int *lev_inds;
} LevelData;

typedef struct {
    Text text;
    AudioResources audio;
    LevelData leveldata;
} Resources;

void LoadResources(Resources *resources, SDL_Renderer *renderer);
void FreeResources(Resources *resources);

#endif /* RESOURCES_H */
