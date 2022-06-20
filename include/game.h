/*
// game.h: contains objects for game itself
*/

#ifndef GAME_H
#define GAME_H

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "consts.h"
#include "utils.h"

#include "resources.h"

#include "solver.h"

typedef struct {
    int id;
    int offsetx;
    int offsety;
    int data[PLAYSPACE_NY][PLAYSPACE_NX];
} PlaySpace;

typedef struct {
    int id;
    int alive;
    int goal_reached;
    int ix;
    int iy;
} Player;

typedef struct {
    Resources *resources;
    PlaySpace spaceleft;
    PlaySpace spaceright;
    Player playerleft;
    Player playerright;
    int status;
    int level;
    int switch_scene_to;
    int next_level;
    int tic;
    Solution *solution;
} GameData;

void InitGame(GameData *game, int level);

// Event handling
void OnEvent_Game(GameData *game, SDL_Event *event, SceneRequest *scenerequest);
int HandlePlayerMove(int new_ix, int new_iy, Player *player, PlaySpace *space);

// Update functions
void OnUpdate_Game(GameData *game, SceneRequest *scenerequest);

// Drawing Functions
void DrawSpace(PlaySpace *space, Player *player, SDL_Renderer *renderer);
void DrawPlayer(Player *player, PlaySpace *space, SDL_Renderer *renderer);
void OnDraw_Game(SDL_Renderer *renderer, GameData *game);

#endif /* GAME_H */
