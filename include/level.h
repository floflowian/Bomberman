/*
 * level.h
 *
 *  Created on: 14 févr. 2013
 *      Author: xderoche
 */

#ifndef LEVEL_H_
#define LEVEL_H_

typedef struct s_level* t_level;

#include "../include/map.h"

// return level number num
t_level level_get_level(int num, t_map map, int num_level, int num_map);

// return current map of the level
t_map level_get_curr_map(t_level level);

// return map number num of the level
t_map level_get_map(t_level level, int num);

// free the level
void level_free(t_level level);

// display the level
void level_display(t_level level, SDL_Surface* screen);
void level_map_inc(t_level level);
void level_reset_cur_map(t_level level);
void level_inc(t_level level);
int level_get_num_level(t_level level);
void level_set_map(t_level level,int num, t_map map);
// free level struct
void level_free(t_level level);
extern int level_get_num_map(t_level level);
#endif /* LEVEL_H_ */
