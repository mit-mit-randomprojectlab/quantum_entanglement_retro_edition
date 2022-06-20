/*
// game.c: contains objects for game itself
*/

#include "game.h"

extern Resources *game_resources;

void InitPlayer(Player *player, int id, int ix, int iy) {
    player->id = id;
    player->ix = ix;
    player->iy = iy;
    
    player->alive = 1;
    player->goal_reached = 0;
}

// InitGame: initialises game variables/objects
void InitGame(GameData *game, int level) {
    
    game->level = level;
    game->next_level = -1;
    
    game->status = 0; // un-finished
    game->switch_scene_to = -1;
    
    // Determine offset locations to "center" spaces
    int space_width = 2*PLAYSPACE_CELLSIZE*PLAYSPACE_NX+PLAYSPACE_CELLSIZE;
    int space_height = PLAYSPACE_CELLSIZE*PLAYSPACE_NY;
    
    game->spaceleft.id = 0;
    game->spaceleft.offsetx = SCREEN_WIDTH/2 - space_width/2;
    game->spaceleft.offsety = SCREEN_HEIGHT/2 - space_height/2;
    
    game->spaceright.id = 1;
    game->spaceright.offsetx = SCREEN_WIDTH/2 - space_width/2 + PLAYSPACE_CELLSIZE*(PLAYSPACE_NX+1);
    game->spaceright.offsety = SCREEN_HEIGHT/2 - space_height/2;
    
    // Initialise Player Coordinates ( (0,0) in left/right playspace by default )
    InitPlayer(&(game->playerleft), 0, 0, 0);
    InitPlayer(&(game->playerright), 1, 0, 0);
    
    // Set level data
    int lind;
    if (game->level >= game_resources->leveldata.nlist) {
        lind = game_resources->leveldata.lev_inds[0];
    }
    else {
        lind = game_resources->leveldata.lev_inds[game->level];
    }
    int (*ldata_left)[PLAYSPACE_NY] = game_resources->leveldata.levels[lind].ldata_left;
    int (*ldata_right)[PLAYSPACE_NY] = game_resources->leveldata.levels[lind].ldata_right;
    
    // Initialise level/space data
    for (int ix = 0; ix < PLAYSPACE_NX; ix++) {
        for (int iy = 0; iy < PLAYSPACE_NY; iy++) {
            if (ldata_left[iy][ix] == 9) {
                game->spaceleft.data[iy][ix] = 0;
                game->playerleft.ix = ix;
                game->playerleft.iy = iy;
            }
            else {
                game->spaceleft.data[iy][ix] = ldata_left[iy][ix];
            }
            if (ldata_right[iy][ix] == 9) {
                game->spaceright.data[iy][ix] = 0;
                game->playerright.ix = ix;
                game->playerright.iy = iy;
            }
            else {
                game->spaceright.data[iy][ix] = ldata_right[iy][ix];
            }
        }
    }
    
    // Pre-calculate solution
    game->solution = solve_level(ldata_left, ldata_right);
    printf("level solution:\n");
    PrintSolution(game->solution);
    
    game->tic = 0;
    
}

int HandlePlayerMove(int new_ix, int new_iy, Player *player, PlaySpace *space) {
    if ( (new_ix >= PLAYSPACE_NX) || (new_iy >= PLAYSPACE_NY) || 
        (new_ix < 0) || (new_iy < 0) || (space->data[new_iy][new_ix] == 1)) { // Hit a wall
        return 0;
    }
    else if (space->data[new_iy][new_ix] == 0) { // move player in free space
        player->ix = new_ix;
        player->iy = new_iy;
        player->goal_reached = 0;
        return 1;
    }
    else if (space->data[new_iy][new_ix] == 2) { // Death trap
        player->ix = new_ix;
        player->iy = new_iy;
        player->alive = 0;
        player->goal_reached = 0;
        return 1;
    }
    else if (space->data[new_iy][new_ix] == 3) { // Goal
        player->ix = new_ix;
        player->iy = new_iy;
        player->goal_reached = 1;
        return 1;
    }
    else {
        printf("Handle move Error ...\n");
        return 0;
    }
}

// OnEvent_Game: handles user input during game
void OnEvent_Game(GameData *game, SDL_Event *event, SceneRequest *scenerequest) {
    if (event->type == SDL_KEYDOWN && event->key.repeat == 0) {
        
        if (game->status == 1) {
            if ((game->level+1) >= game_resources->leveldata.nlist) {
                scenerequest->switch_scene_to = 0; // back to title
                scenerequest->flag = 0;
            }
            else {
                scenerequest->switch_scene_to = 1;
                scenerequest->flag = game->level+1;
            }
            return;
        }
        else if (game->status == 2) {
            scenerequest->switch_scene_to = 1;
            scenerequest->flag = game->level;
            return;
        }
        
        int mc = 0;
        int solmove = -1;
        if (event->key.keysym.sym == SDLK_SPACE) {
            if ( (game->solution->moves[0] >= 0) && (game->solution->current < game->solution->n) ) {
                solmove = game->solution->moves[game->solution->current];
                game->solution->current++;
            }
            else {
                Mix_PlayChannel( -1, game_resources->audio.sound_fail, 0 );
            }
        }
        if ( (event->key.keysym.sym == SDLK_UP) || (solmove == 2) ) {
            mc += HandlePlayerMove(game->playerleft.ix, game->playerleft.iy-1, &(game->playerleft), &(game->spaceleft));
            mc += HandlePlayerMove(game->playerright.ix, game->playerright.iy-1, &(game->playerright), &(game->spaceright));
		}
		else if ( (event->key.keysym.sym == SDLK_DOWN) || (solmove == 3) ) {
            mc += HandlePlayerMove(game->playerleft.ix, game->playerleft.iy+1, &(game->playerleft), &(game->spaceleft));
            mc += HandlePlayerMove(game->playerright.ix, game->playerright.iy+1, &(game->playerright), &(game->spaceright));
		}
		else if ( (event->key.keysym.sym == SDLK_LEFT) || (solmove == 0) ) {
            mc += HandlePlayerMove(game->playerleft.ix-1, game->playerleft.iy, &(game->playerleft), &(game->spaceleft));
            mc += HandlePlayerMove(game->playerright.ix-1, game->playerright.iy, &(game->playerright), &(game->spaceright));
		}
		else if ( (event->key.keysym.sym == SDLK_RIGHT) || (solmove == 1) ) {
            mc += HandlePlayerMove(game->playerleft.ix+1, game->playerleft.iy, &(game->playerleft), &(game->spaceleft));
            mc += HandlePlayerMove(game->playerright.ix+1, game->playerright.iy, &(game->playerright), &(game->spaceright));
		}
		if (mc > 0) { // any movement
		    Mix_PlayChannel( -1, game_resources->audio.sound_step, 0 );
		}
    }
}

/*
// Game update functions
*/

// OnUpdate_Game: handles general game updates
void OnUpdate_Game(GameData *game, SceneRequest *scenerequest) {
    
    // Check for win/lose states
    if (game->status == 0) {
        if ( (game->playerleft.alive == 0) || (game->playerright.alive == 0) ) {
            game->status = 2;
            Mix_PlayChannel( -1, game_resources->audio.sound_fail, 0 );
        }
        else if ( (game->playerleft.goal_reached == 1) && (game->playerright.goal_reached == 1) ) {
            game->status = 1;
            Mix_PlayChannel( -1, game_resources->audio.sound_complete, 0 );
        }
    }
    
    if (scenerequest->switch_scene_to >= 0) {
        FreeSolution(game->solution);
    }
    
    game->tic++;
    
}

/*
// Game drawing functions
*/

void DrawSpace(PlaySpace *space, Player *player, SDL_Renderer *renderer) {
    
    // Outer Edge
    SDL_Rect outer = set_rect(space->offsetx, space->offsety,
        PLAYSPACE_CELLSIZE*PLAYSPACE_NX,PLAYSPACE_CELLSIZE*PLAYSPACE_NY);
    
    if (player->goal_reached == 1) {
        SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
        SDL_RenderFillRect(renderer, &outer);
    }
    else if (player->alive == 0) {
        SDL_SetRenderDrawColor(renderer, 80, 0, 0, 255);
        SDL_RenderFillRect(renderer, &outer);
    }
    
    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &outer);
    
    // Cells
    for (int ix = 0; ix < PLAYSPACE_NX; ix++) {
        for (int iy = 0; iy < PLAYSPACE_NY; iy++) {
            int x = space->offsetx + ix*PLAYSPACE_CELLSIZE + 1;
            int y = space->offsety + iy*PLAYSPACE_CELLSIZE + 1;
            SDL_Rect r = set_rect(x,y,PLAYSPACE_CELLSIZE-2,PLAYSPACE_CELLSIZE-2);
            
            switch (space->data[iy][ix]) {
                case 0:
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // clear space
                    SDL_RenderDrawRect(renderer, &r);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // occupied space
                    SDL_RenderFillRect(renderer, &r);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // danger, Will Robinson!
                    SDL_RenderFillRect(renderer, &r);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Gooooooaaaal!
                    SDL_RenderFillRect(renderer, &r);
                    break;
                default:
                    printf("Bad cell data! ix: %d, iy: %d\n",ix,iy);
                    break;
            }
        }
    }
    
}

void DrawPlayer(Player *player, PlaySpace *space, SDL_Renderer *renderer) {
    
    int x = space->offsetx + player->ix*PLAYSPACE_CELLSIZE + PLAYSPACE_CELLSIZE/2;
    int y = space->offsety + player->iy*PLAYSPACE_CELLSIZE + PLAYSPACE_CELLSIZE/2;
    if (player->id == 0) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    }
    DrawCircle(renderer, x, y, PLAYSPACE_CELLSIZE/2-4);
    
}

void OnDraw_Game(SDL_Renderer *renderer, GameData *game) {
    
    // win/lose states
    int alpha = (int)(255*(0.5*sin(8.0*((float)game->tic)/30.0)+0.5));
    if (game->status == 1) { // win
        SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect dest = { SCREEN_WIDTH/2-game_resources->text.complete.w/2, 100, 
            game_resources->text.complete.w, game_resources->text.complete.h };
        SDL_SetTextureAlphaMod(game_resources->text.complete.texture, alpha);
        SDL_RenderCopy(renderer, game_resources->text.complete.texture, NULL, &dest);
    }
    else if (game->status == 2) { // lose
        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Rect dest = { SCREEN_WIDTH/2-game_resources->text.failed.w/2, 100, 
            game_resources->text.failed.w, game_resources->text.failed.h };
        SDL_SetTextureAlphaMod(game_resources->text.failed.texture, alpha);
        SDL_RenderCopy(renderer, game_resources->text.failed.texture, NULL, &dest);
    }
    
    // Draw game spaces
    DrawSpace(&(game->spaceleft),&(game->playerleft),renderer);
    DrawSpace(&(game->spaceright),&(game->playerright),renderer);
    
    // Draw players
    DrawPlayer(&(game->playerleft),&(game->spaceleft),renderer);
    DrawPlayer(&(game->playerright),&(game->spaceright),renderer);
    
    // Start text
    if (game->tic < 30) {
        float size = 20*pow((game->tic/30.0),2);
        int w = (int)(size*game_resources->text.start.w);
        int h = (int)(size*game_resources->text.start.h);
        SDL_Rect dest = {SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT/2-h/2, w, h};
        int alpha = 255*((30-game->tic)/30.0);
        SDL_SetTextureAlphaMod(game_resources->text.start.texture, alpha);
        SDL_RenderCopy(renderer, game_resources->text.start.texture, NULL, &dest);
    }
    
    // Update Screen
    SDL_RenderPresent(renderer);
}

