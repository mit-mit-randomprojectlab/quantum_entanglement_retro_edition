/*
// main.c: main application/game
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"
#include "consts.h"

#include "scene.h"
#include "title.h"
#include "game.h"

/*
// Entry point for running game/app
*/
int main() {
    
    // Setup Game SDL2 Window
    GameScene scene;
    InitScene(&scene);
    
    // Setup initial Game Data
    GameData game;
    
    // Run game Scene
    //SwitchScene(&scene, &game, SCENE_MAINGAME, 0);
    SwitchScene(&scene, &game, SCENE_TITLE, 0);
    RunScene(&scene, &game);
    EndScene(&scene);
    
    return 0;
}
