/*
// resources.h: store reference to resources such as gfx, sfx and music
*/

#include "resources.h"

void LoadTexture_wh(SDL_Renderer *renderer, SDL_Surface *surf, Texture_wh *texture_wh) {
    texture_wh->texture = SDL_CreateTextureFromSurface(renderer, surf);
    texture_wh->w = surf->w;
    texture_wh->h = surf->h;
}

void PrintLevel(Level *level) {
    printf("%s\n",level->name);
    for (int j = 0; j < 8; j++) {
        for (int k = 0; k < 8; k++) {
            printf("%d,",level->ldata_left[j][k]);
        }
        printf("\n");
    }
    printf("\n");
    for (int j = 0; j < 8; j++) {
        for (int k = 0; k < 8; k++) {
            printf("%d,",level->ldata_right[j][k]);
        }
        printf("\n");
    }
}

void LoadLevelData(const char *path, LevelData *leveldata) {
    
    FILE *fp = fopen(path,"r");
    
    // determine number of levels
    char buffer[256];
    leveldata->n = 0;
    while (fgets(buffer, 256, fp)) {
        leveldata->n++;
    }
    leveldata->n = (leveldata->n-2)/19;    
    leveldata->levels = (Level*)malloc(leveldata->n*sizeof(Level));
    
    // Read in all leveldata
    rewind(fp);
    fgets(buffer, 256, fp);
    fgets(buffer, 256, fp);
    
    for (int i = 0; i < leveldata->n; i++) {
        fgets(buffer, 256, fp);
        sscanf(buffer,"%s",leveldata->levels[i].name);
        for (int j = 0; j < 8; j++) {
            fgets(buffer, 256, fp);
            for (int k = 0; k < 8; k++) {
                sscanf(buffer+2*k,"%d,",&(leveldata->levels[i].ldata_left[j][k]));
            }
        }
        fgets(buffer, 256, fp);
        for (int j = 0; j < 8; j++) {
            fgets(buffer, 256, fp);
            for (int k = 0; k < 8; k++) {
                sscanf(buffer+2*k,"%d,",&(leveldata->levels[i].ldata_right[j][k]));
            }
        }
        fgets(buffer, 256, fp);
    }
    
    // Read level list
    rewind(fp);
    fgets(buffer, 2048, fp);
    char *p = strtok(buffer, ",");
    leveldata->nlist = 0;
    while (p != NULL) {
        p = strtok(NULL, ",");
        leveldata->nlist++;
    }
    leveldata->lev_inds = (int*)malloc(leveldata->nlist*sizeof(int));
    
    // Assign list of indicies to level data
    rewind(fp);
    fgets(buffer, 2048, fp);
    p = strtok(buffer, ",");
    int lind = 0;
    while (p != NULL) {
        if (p[strlen(p)-1] == '\n') {
            p[strlen(p)-1] = '\0';
        }
        for (int i = 0; i < leveldata->n; i++) {
            if (strcmp(leveldata->levels[i].name, p) == 0) {
                leveldata->lev_inds[lind] = i;
                lind++;
                break;
            }
            else if (i == (leveldata->n-1)) {
                printf("Can't find data for level: %s\n", p);
                leveldata->lev_inds[lind] = -1;
                lind++;
            }
        }
        p = strtok(NULL, ",");
    }
    /*for (int i = 0; i < leveldata->nlist; i++) {
        printf("%s\n",leveldata->levels[leveldata->lev_inds[i]].name);
    }*/
    //PrintLevel(&(leveldata->levels[leveldata->lev_inds[0]]));
    
    fclose(fp);
    
}

void FreeLevelData(LevelData *leveldata) {
    free(leveldata->levels);
    free(leveldata->lev_inds);
}

void LoadResources(Resources *resources, SDL_Renderer *renderer) {
    
    // Font
    resources->text.font = TTF_OpenFont("data/font/TrenchThin-aZ1J.ttf", 32);
    
    // gfx and textures
    SDL_Color color0 = {200, 200, 200, 255};
    SDL_Surface *text0 = TTF_RenderText_Solid(resources->text.font, "Start!", color0);
    LoadTexture_wh(renderer, text0, &(resources->text.start));
    SDL_FreeSurface(text0);
    
    SDL_Color color = {0, 200, 0, 255};
    SDL_Surface *text = TTF_RenderText_Solid(resources->text.font, "Complete!", color);
    LoadTexture_wh(renderer, text, &(resources->text.complete));
    SDL_FreeSurface(text);
    
    SDL_Color color2 = {200, 0, 0, 255};
    SDL_Surface *text2 = TTF_RenderText_Solid(resources->text.font, "Failed!", color2);
    LoadTexture_wh(renderer, text2, &(resources->text.failed));
    SDL_FreeSurface(text2);
    
    // Load sfx and music
    resources->audio.music_A = Mix_LoadMUS("data/music/scientists_remix.ogg");
    resources->audio.sound_step = Mix_LoadWAV("data/sfx/select_simple.wav");
    resources->audio.sound_fail = Mix_LoadWAV("data/sfx/alarm_novoice.wav");
    resources->audio.sound_complete = Mix_LoadWAV("data/sfx/gem.wav");
    
    // Load level data
    LoadLevelData("data/levels.dat", &(resources->leveldata));
    
    // Adjust volumes
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(resources->audio.sound_step, MIX_MAX_VOLUME/4);
}

void FreeResources(Resources *resources) {
    
    TTF_CloseFont(resources->text.font);
    SDL_DestroyTexture(resources->text.start.texture);
    SDL_DestroyTexture(resources->text.complete.texture);
    SDL_DestroyTexture(resources->text.failed.texture);
    
    Mix_FreeMusic(resources->audio.music_A);
    Mix_FreeChunk(resources->audio.sound_step);
    Mix_FreeChunk(resources->audio.sound_fail);
    Mix_FreeChunk(resources->audio.sound_complete);
    
    FreeLevelData(&(resources->leveldata));
    
}
