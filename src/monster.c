#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/monster.h"
#include "../include/game.h"

struct s_monster {
    int x, y, timer;
    SDL_Surface * directed_img[4];
    enum e_way current_way;

};

t_monster monster_init(int x, int y,int type) {
    t_monster monster = malloc(sizeof (*monster));
    if (!monster)
        error("Memory error");
    if(type==1){
    monster->directed_img[WEST] = load_image(IMG_MONSTER_LEFT);
    monster->directed_img[EAST] = load_image(IMG_MONSTER_RIGHT);
    monster->directed_img[NORTH] = load_image(IMG_MONSTER_UP);
    monster->directed_img[SOUTH] = load_image(IMG_MONSTER_DOWN);
    }
    else if(type==2){
         monster->directed_img[WEST] = load_image(IMG_MONSTER2_LEFT);
    monster->directed_img[EAST] = load_image(IMG_MONSTER2_RIGHT);
    monster->directed_img[NORTH] = load_image(IMG_MONSTER2_UP);
    monster->directed_img[SOUTH] = load_image(IMG_MONSTER2_DOWN);
    }
    monster->current_way = SOUTH;
    monster->x = x;
    monster->y = y;
    monster->timer = 0;


    return monster;
}

int monster_get_timer(t_monster monster) {

    return monster->timer;
}

void monster_reset_timer(t_monster monster) {

    monster->timer = 0;
}

void monster_inc_timer(t_monster monster) {

    monster->timer += 1;
}

void monster_free(t_monster monster) {
    assert(monster);
    int i;
    for (i = 0; i < 4; i++)
        SDL_FreeSurface(monster->directed_img[i]);

    free(monster);
}

static int monster_move_aux(t_monster monster, t_map map, int x1, int y1, int x2, int y2) {

    if (!map_is_inside(map, x1, y1))
        return 0;


    switch (map_get_cell_type(map, x1, y1)) {
        case CELL_SCENERY:
            return 0;
            break;

        case CELL_CASE:
            return 0;
            break;

        case CELL_BONUS:
            return 0;
            break;

        case CELL_GOAL:
            return 0;
            break;

         case CELL_BOMB:
            return 0;
            break;
            
        case CELL_MONSTER:
            break;

        case CELL_PLAYER:
            break;

        case CELL_EMPTY:

            break;

        case CELL_CLOSED_DOOR:
            return 0;
            break;
        case CELL_DOOR:
            return 0;
            break;

        case CELL_KEY:
            return 0;
            break;

        default:
            break;
    }
    return 1;
}

int monster_get_x(t_monster monster) {
    assert(monster != NULL);
    return monster->x;
}

int monster_get_y(t_monster monster) {
    assert(monster != NULL);
    return monster->y;
}

void monster_set_current_way(t_monster monster,t_map map) {
    assert(monster);
    int x=monster_get_x( monster);
    int y=monster_get_y( monster);
    int way=random_direction();
    if(map_get_cell_type(map,x+1,y)==CELL_KEY || map_get_cell_type(map,x+1,y)==CELL_DOOR || map_get_cell_type(map,x+1,y)==CELL_KEY || map_get_cell_type(map,x+1,y)==CELL_CLOSED_DOOR || 
            map_get_cell_type(map,x+1,y)==CELL_GOAL || map_get_cell_type(map,x+1,y)==CELL_CASE || map_get_cell_type(map,x+1,y)==CELL_SCENERY || map_get_cell_type(map,x+1,y)==CELL_BOMB || x==11   )
    {
        
        while(way==3){
            way=random_direction();
        }
        monster->current_way =way ;
    }
    else if(map_get_cell_type(map,x-1,y)==CELL_KEY || map_get_cell_type(map,x-1,y)==CELL_DOOR || map_get_cell_type(map,x-1,y)==CELL_KEY || map_get_cell_type(map,x-1,y)==CELL_CLOSED_DOOR || 
            map_get_cell_type(map,x-1,y)==CELL_GOAL || map_get_cell_type(map,x-1,y)==CELL_CASE || map_get_cell_type(map,x-1,y)==CELL_SCENERY || map_get_cell_type(map,x-1,y)==CELL_BOMB || x==0  )
    {
        while(way==2){
            way=random_direction();
        }
        monster->current_way =way ;
    }
     else if(map_get_cell_type(map,x,y+1)==CELL_KEY || map_get_cell_type(map,x,y+1)==CELL_DOOR || map_get_cell_type(map,x,y+1)==CELL_KEY || map_get_cell_type(map,x,y+1)==CELL_CLOSED_DOOR || 
            map_get_cell_type(map,x,y+1)==CELL_GOAL || map_get_cell_type(map,x,y+1)==CELL_CASE || map_get_cell_type(map,x,y+1)==CELL_SCENERY   || map_get_cell_type(map,x,y+1)==CELL_BOMB )
    {
        while(way==1){
            way=random_direction();
        }
        monster->current_way =way ;
    }
     else if(map_get_cell_type(map,x,y-1)==CELL_KEY || map_get_cell_type(map,x,y-1)==CELL_DOOR || map_get_cell_type(map,x,y-1)==CELL_KEY || map_get_cell_type(map,x,y-1)==CELL_CLOSED_DOOR || 
            map_get_cell_type(map,x,y-1)==CELL_GOAL || map_get_cell_type(map,x,y-1)==CELL_CASE || map_get_cell_type(map,x,y-1)==CELL_SCENERY  || map_get_cell_type(map,x,y-1)==CELL_BOMB  )
    {
        while(way==0){
            way=random_direction();
        }
        
        monster->current_way =way ;
    }
     else monster->current_way =random_direction() ;
    
    
    
}

void monster_from_map(t_monster monster, t_map map) {
    int i, j;
    for (i = 0; i < map_get_width(map); i++) {
        for (j = 0; j < map_get_height(map); j++) {
            if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
                monster->x = i;
                monster->y = j;
            }
        }
    }
}

int monster_move(t_monster monster, t_map map) {
    int x = monster->x;
    int y = monster->y;
    int move = 0;

    switch (monster->current_way) {
        case NORTH:

            if (monster->y - 1 < 0) break;
            if (monster_move_aux(monster, map, x, y - 1, x, y - 2)) {
                monster->y--;

                move = 1;
            }
            break;

        case SOUTH:
            if (monster->y + 1 > 11) break;
            if (monster_move_aux(monster, map, x, y + 1, x, y + 2)) {

                monster->y++;
                move = 1;
            }
            break;

        case WEST:
            if (monster->x - 1 < 0) break;
            if (monster_move_aux(monster, map, x - 1, y, x - 2, y)) {

                monster->x--;
                move = 1;
            }
            break;

        case EAST:
            if (monster->x + 1 > 11) break;
            if (monster_move_aux(monster, map, x + 1, y, x + 2, y)) {
                monster->x++;
                move = 1;
            }
            break;
    }

    if (move) {
        map_set_cell_type(map, x, y, CELL_EMPTY);
        map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
    }
    return move;
}

void monster_display(t_monster monster, SDL_Surface *screen) {
    assert(monster);
    assert(monster->directed_img[monster->current_way]);
    assert(screen);

    SDL_Rect place;
    place.x = monster->x * SIZE_BLOC;
    place.y = monster->y * SIZE_BLOC;

    SDL_BlitSurface(monster->directed_img[monster->current_way], NULL, screen, &place);
}

int random_direction() {
    return rand() % (4 - 0) + 0;
}

