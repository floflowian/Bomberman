/*
 * map.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent Reveillere
 */

#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"

typedef struct s_map* t_map;
typedef struct s_tab* t_tab;

#include "../include/level.h"

typedef enum {
    CELL_EMPTY = 0,
    CELL_GOAL, // 1
    CELL_SCENERY, // 2
    CELL_PLAYER, // 3
    CELL_CASE, // 4
    CELL_BONUS, // 5
    CELL_MONSTER, // 6
    CELL_BOMB, // 7
    CELL_KEY, // 8
    CELL_DOOR, // 9
    CELL_CLOSED_DOOR, // 10
    CELL_BOMBERWOMAN //11
} t_cell_type;

typedef enum {
    BONUS_BOMB_RANGE_INC = 1,
    BONUS_BOMB_RANGE_DEC,
    BONUS_BOMB_NB_INC,
    BONUS_BOMB_NB_DEC,
    BONUS_LIFE,
    BONUS_MONSTER
} t_bonus_type;

typedef enum {
    SCENERY_STONE = 1, SCENERY_TREE
} t_scenery_type;

typedef enum {
    CELL_STONE = CELL_SCENERY | (SCENERY_STONE << 4),
    CELL_TREE = CELL_SCENERY | (SCENERY_TREE << 4),
    CELL_CASE_BOMBINC = CELL_BONUS | (BONUS_BOMB_NB_INC << 4),
    CELL_CASE_BOMBDEC = CELL_BONUS | (BONUS_BOMB_NB_DEC << 4),
    CELL_CASE_RANGEINC = CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4),
    CELL_CASE_RANGEDEC = CELL_BONUS | (BONUS_BOMB_RANGE_DEC << 4),
    CELL_CASE_LIFE = CELL_BONUS | (BONUS_LIFE << 4),
    CELL_CASE_MONSTER = CELL_CASE | (BONUS_MONSTER << 4)
} t_compose_type;



// Create a new empty map
extern t_map map_new(int width, int height);
extern void map_free(t_map map);
extern t_map map_get_default(t_level level);

// Return the height and width of a map
extern int map_get_width(t_map map);
extern int map_get_height(t_map map);
extern void screen_editor(SDL_Surface* screen, int theplacecursor);

// Return the type of a cell
extern t_cell_type map_get_cell_type(t_map map, int x, int y);
extern t_bonus_type map_get_bonus_type(t_map map, int x, int y);

// Set the type of a cell
extern void map_set_cell_type(t_map map, int x, int y, t_cell_type type);
extern void map_set_grid(t_map map, int i, int themap);
// Test if (x,y) is within the map
extern int map_is_inside(t_map map, int x, int y);

// Load a static pre-calculated map
//extern t_map map_get_default(t_level level);

// Display the map on the screen
extern void map_display(t_map map, SDL_Surface *screen);
extern void move_cell(int place_x, int place_y, SDL_Surface * screen, SDL_Surface * sprite, t_tab tab, int theplacecursor, SDL_Rect totalplace[100], SDL_Surface * cell[100], int current_cell);
int map_editor(SDL_Surface *screen);
#endif /* MAP_H_ */
