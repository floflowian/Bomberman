/*
 * bomb.h
 *
 *  Created on: 26 févr. 2013
 *      Author: flouistherese
 */

#ifndef BOMB_H_
#define BOMB_H_

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"
#include "../include/game.h"
#include "../include/player.h"


typedef struct s_bomb * t_bomb;

extern void bomb_free(t_bomb bomb);
extern void bomb_set_timer(t_bomb bomb);
extern t_bomb bomb_init();

//extern void bomb_set_place(t_bomb bomb,t_player player);
#endif /* BOMB_H_ */

