/*
// scene.c: Controls main SDL window stuff
*/

#include "scene.h"

/*
// Main scene functions
*/

Resources *game_resources = NULL;

void InitScene(GameScene *scene) {
    
    // Initialise video/audio
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) != 0 ) {
	    printf("Failed to open audio: %s", Mix_GetError());
    }
    TTF_Init();
    
    // Start-up main window
    scene->window = SDL_CreateWindow("Quantum Entanglement: Retro Edition", 0, 0, 
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(scene->window);
    
    scene->renderer = NULL;
    scene->renderer = SDL_CreateRenderer(scene->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(scene->renderer, 0, 0, 0, 255);
    SDL_RenderClear(scene->renderer);
    
    LoadResources(&(scene->resources), scene->renderer);
    game_resources = &(scene->resources);
    
    // Start music
    Mix_PlayMusic(scene->resources.audio.music_A, -1);
    
    scene->running = 1;
    scene->sceneid = SCENE_MAINGAME;
}
void EndScene(GameScene *scene) {
    
    FreeResources(&(scene->resources));
    
    SDL_DestroyWindow(scene->window);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void RunScene(GameScene *scene, GameData *game) {
    SDL_Event event;
    while(scene->running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                scene->running = 0;
            }
            else {
                OnEvent(scene, game, &event);
            }
        }
        OnUpdate(scene, game);
        OnDraw(scene, game);
        
        if (game->switch_scene_to >= 0) {
            SwitchScene(scene, game, game->switch_scene_to, game->next_level); // Reset for now
        }
        
        SDL_Delay(32);
    }
}

void SwitchScene(GameScene *scene, GameData *game, int new_scene, int flag) {
    scene->sceneid = new_scene;
    
    switch (scene->sceneid) {
        case SCENE_MAINGAME:
            printf("starting level %d ...\n", flag);
            InitGame(game, flag);
            break;
        default:
            printf("Invalid Scene ID (switch)\n");
            break;
    }
}

void OnEvent(GameScene *scene, GameData *game, SDL_Event *event) {
    
    switch (scene->sceneid) {
        case SCENE_MAINGAME:
            OnEvent_Game(game, event);
            break;
        default:
            printf("Invalid Scene ID (event)\n");
            break;
    }
    
}

void OnUpdate(GameScene *scene, GameData *game) {
    
    switch (scene->sceneid) {
        case SCENE_MAINGAME:
            OnUpdate_Game(game);
            break;
        default:
            printf("Invalid Scene ID (update)\n");
            break;
    }
    
}

void OnDraw(GameScene *scene, GameData *game) {
    
    SDL_SetRenderDrawColor(scene->renderer, 0, 0, 0, 255);
    SDL_RenderClear(scene->renderer);
    switch (scene->sceneid) {
        case SCENE_MAINGAME:
            OnDraw_Game(scene->renderer, game);
            break;
        default:
            printf("Invalid Scene ID (draw)\n");
            break;
    }
    
}



