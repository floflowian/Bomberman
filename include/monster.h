/* 
 * File:   monster.h
 * Author: florian
 *
 * Created on March 7, 2013, 12:24 AM
 */

#ifndef MONSTER_H
#define	MONSTER_H

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "../include/constant.h"
#include "../include/map.h"


typedef struct s_monster * t_monster;

extern t_monster monster_init();
extern void monster_free(t_monster monster);
extern int monster_get_timer(t_monster monster);
extern void monster_reset_timer(t_monster monster);
extern void monster_inc_timer(t_monster monster);
extern int monster_get_x(t_monster monster);
extern int monster_get_y(t_monster monster);
extern void monster_set_current_way(t_monster monster, t_map map);
extern void monster_from_map(t_monster monster, t_map map);
extern int monster_move(t_monster monster, t_map map);
extern void monster_display(t_monster monster, SDL_Surface *screen);

extern int random_direction();






#endif	/* MONSTER_H */

