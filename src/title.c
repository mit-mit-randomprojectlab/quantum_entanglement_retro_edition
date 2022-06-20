/*
// title.c: contains objects for titlescreen etc.
*/

#include "title.h"

extern Resources *game_resources;

static int status;
static int tic;

void InitTitle(void) {
    status = 0;
    tic = 0;
}

// Event handling
void OnEvent_Title(SDL_Event *event, SceneRequest *scenerequest) {
    if ( (event->type == SDL_KEYDOWN && event->key.repeat == 0) && (status == 0) ) {
        status = 1;
        scenerequest->switch_scene_to = 1;
        scenerequest->flag = 0;
        Mix_PlayChannel( -1, game_resources->audio.sound_complete, 0 );
    }
}

// Update functions
void OnUpdate_Title(SceneRequest *scenerequest) {
    tic++;
}

// Drawing Functions
void OnDraw_Title(SDL_Renderer *renderer) {
    int alpha = (int)(255*(0.3*sin(4.0*((float)tic)/30.0)+0.7));
    
    SDL_Rect dest = { SCREEN_WIDTH/2-game_resources->text.title.w/2, 100, 
        game_resources->text.title.w, game_resources->text.title.h };
    SDL_SetTextureAlphaMod(game_resources->text.title.texture, 255);
    SDL_RenderCopy(renderer, game_resources->text.title.texture, NULL, &dest);
    
    SDL_Rect dest2 = { SCREEN_WIDTH/2-game_resources->text.title2.w/2, 200, 
        game_resources->text.title2.w, game_resources->text.title2.h };
    SDL_SetTextureAlphaMod(game_resources->text.title2.texture, alpha);
    SDL_RenderCopy(renderer, game_resources->text.title2.texture, NULL, &dest2);
    
    // Update Screen
    SDL_RenderPresent(renderer);
}

