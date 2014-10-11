/*
 * game.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent Reveillere
 */

#ifndef GAME_H_
#define GAME_H_

#include "../include/constant.h"
#include "../include/misc.h"

#include "../include/map.h"
#include "../include/player.h"
#include "../include/level.h"
#include "../include/bomb.h"

typedef struct s_game * t_game;

// Create a new game
extern t_game game_new(int choice);

//
extern void game_free(t_game game);

// Return the player of the current game
extern t_player game_get_player(t_game game);

// Return the current level
extern t_level game_get_curr_level(t_game game);

// Display the game of the screen
extern void game_display(t_game game, SDL_Surface *screen,SDL_Surface* grass);
extern int splashscreen(SDL_Surface *screen, int num);
// update
extern int game_update(t_game game, SDL_Surface* screen);
extern int check_monster_player(t_game game, int autorisation, int j);
t_compose_type random_bonus(t_game game, int x, int y);
extern void pause();
extern int player_check_finish(t_player player);
extern void save(t_game game);
#endif /* GAME_H_ */
