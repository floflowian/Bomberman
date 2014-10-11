/*
 * player.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"
#include "../include/bomb.h"

typedef struct s_game * t_game;

enum e_way {
    NORTH = 0, SOUTH, WEST, EAST
};

typedef struct s_player * t_player;



// Creates a new player with a given number of available bombs
extern t_player player_init();
extern void player_free(t_player player);



// Returns the current position of the player
extern int player_get_x(t_player player);
extern int player_get_y(t_player player);
extern int get_life(t_player player);

// Set the direction of the next move of the player
extern void player_set_current_way(t_player player, enum e_way way);

// Set, Increase, Decrease the number of bomb that player can put
extern void player_bonus(t_player player, t_map map, int x, int y);
extern int player_get_nb_bomb(t_player player);
extern void player_inc_nb_bomb(t_player player);
extern void player_dec_nb_bomb(t_player player);
extern int player_get_total_nb_bomb_set(t_player player);
extern int player_get_curr_nb_bomb_set(t_player player);
extern void player_inc_total_nb_bomb_set(t_player player);
extern void player_inc_curr_nb_bomb_set(t_player player);
extern void player_dec_curr_nb_bomb_set(t_player player);
extern void player_dec_life(t_player player);
extern void player_set_y(t_player player, int y);
extern void player_set_x(t_player player, int x);
extern int player_get_nb_key(t_player player);
extern void player_reset_nb_key(t_player player);
extern void player_set_life(t_player player, int nb_life);
extern void player_set_power(t_player player, int num);
extern void player_set_nb_key(t_player player, int num);
extern void player_set_nb_bomb(t_player player, int num);
extern void player_reset_bonus(t_player player);
// Load the player position from the map
extern void player_from_map(t_player player, t_map map);

// Move the player according to the current direction
extern int player_move(t_player player, t_map map,t_game game);

// Display the player on the screen
extern void player_display(t_player player, SDL_Surface *screen);

extern int bomb_display(t_game game, t_player player, t_bomb bomb, SDL_Surface *screen, t_map map);

int get_player_power(t_player player);

void bomb_set_place(t_bomb bomb, t_player player);

#endif /* PLAYER_H_ */
