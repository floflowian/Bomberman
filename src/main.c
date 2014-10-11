/*
 * main.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include <SDL.h>
#include <SDL_image.h>

#include "../include/game.h"

int main(int argc, char *argv[]) {
    SDL_Surface *screen;
    int i, j;
    int choice = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        error("Can't init SDL:  %s\n", SDL_GetError());
    }

    t_game game = game_new(choice);
    t_player player = game_get_player(game);
    t_map map = level_get_curr_map(game_get_curr_level(game));
     
    SDL_Surface* grass=IMG_Load(IMG_GRASS);
    SDL_WM_SetCaption(WINDOW_NAME, NULL);
    screen = SDL_SetVideoMode(
            SIZE_BLOC * 12,
            SIZE_BLOC * 12 + SIZE_BANNER + SIZE_LINE,
            WINDOW_BPP,
            SDL_HWSURFACE);
    
    choice = splashscreen(screen, 0);
    
    if(choice==4){
        
       game_free(game);
    SDL_FreeSurface(screen);
    SDL_Quit();
     return EXIT_SUCCESS;
    }

   game = game_new(choice);
    player = game_get_player(game);
    map = level_get_curr_map(game_get_curr_level(game));
    
    if (screen == NULL) {
        error("Can't set video mode: %s\n", SDL_GetError());
    }
    int type = map_get_cell_type(map, 0, 1);

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 12; j++) {
            type = map_get_cell_type(map, i, j);

            switch (type) { // type is encoded with 4 bits (the & 15 (1111)) is a mask to keep the four less significant bits)

                case 3:
                    //SDL_Quit();
                    player_set_y(player, j);
                    player_set_x(player, i);
                    break;
            }
        }
    }
    
    game_display(game, screen,grass);

    int done = 0;

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    // to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
    uint ideal_speed = 1000 / DEFAULT_GAME_FPS;
    Uint32 timer, execution_speed;
    //int i=0;
    // game loop
    // fixed time rate implementation
    while (!done) {

        timer = SDL_GetTicks();

        done = game_update(game, screen);
        game_display(game, screen,grass);

        execution_speed = SDL_GetTicks() - timer;
        if (execution_speed < ideal_speed) {
            SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
        }
        i++;
    }

    game_free(game);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return EXIT_SUCCESS;
}
