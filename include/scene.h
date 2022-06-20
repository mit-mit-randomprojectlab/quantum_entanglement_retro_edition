/*
// scene.h: Controls main SDL window stuff
*/

#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "consts.h"
#include "utils.h"

#include "resources.h"

#include "game.h"

#define SCENE_TITLE 0
#define SCENE_MAINGAME 1

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Resources resources;
    int sceneid;
    int running;
} GameScene;

void InitScene(GameScene *scene);
void EndScene(GameScene *scene);

void RunScene(GameScene *scene, GameData *game);
void SwitchScene(GameScene *scene, GameData *game, int new_scene, int flag);

void OnEvent(GameScene *scene, GameData *game, SDL_Event *event);
void OnUpdate(GameScene *scene, GameData *game);
void OnDraw(GameScene *scene, GameData *game);

#endif /* SCENE_H */
