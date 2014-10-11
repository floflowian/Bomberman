/*
 * player.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent Reveillere
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/game.h"
#include "../include/bomb.h"

struct s_bomb {
    int x, y, timer, power, isset;
    SDL_Surface * img_bomb;
    SDL_Surface * explosion;
};

struct s_player {
    int x, y, life,next_bomb;
    SDL_Surface * directed_img[4];
    enum e_way current_way;
    int nb_bomb;
    int curr_nb_bomb_set;
    int total_nb_bomb_set;
    int power;
    int key;
    int finish;
};

t_player player_init() {//sets a new player with the basic assets
    t_player player = malloc(sizeof (*player));
    if (!player)
        error("Memory error");
  
    player->x = 0;
    player->y = 0;
    player->next_bomb=0;
    player->directed_img[WEST] = load_image(IMG_PLAYER_LEFT);
    player->directed_img[EAST] = load_image(IMG_PLAYER_RIGHT);
    player->directed_img[NORTH] = load_image(IMG_PLAYER_UP);
    player->directed_img[SOUTH] = load_image(IMG_PLAYER_DOWN);
    player->current_way = SOUTH;
    player->nb_bomb = 3;
    player->curr_nb_bomb_set = 0;
    player->total_nb_bomb_set = 0;
    player->life = 2;
    player->power = BOMB_RANGE_DEFAULT;
    player->key = 0;
    player->finish = 0;
    // }
    return player;
}

void player_free(t_player player) {
    assert(player);
    int i;
    for (i = 0; i < 4; i++)
        SDL_FreeSurface(player->directed_img[i]);

    free(player);
}

int player_get_x(t_player player) {
    assert(player != NULL);
    return player->x;
}

void player_set_x(t_player player, int x) {
    assert(player != NULL);
    player->x = x;
}

int player_get_y(t_player player) {
    assert(player != NULL);
    return player->y;
}

void player_set_y(t_player player, int y) {
    assert(player != NULL);
    player->y = y;
}

void player_set_current_way(t_player player, enum e_way way) {
    assert(player);
    player->current_way = way;
}
int player_get_current_way(t_player player) {
    assert(player);
    return player->current_way;
}

int player_get_nb_bomb(t_player player) {
    assert(player);
    return player->nb_bomb;
}

void player_set_nb_bomb(t_player player, int num) {
    assert(player);
    player->nb_bomb = num;
}

int player_get_nb_key(t_player player) {
    assert(player);
    return player->key;
}

void player_set_nb_key(t_player player, int num) {
    assert(player);
    player->key = num;
}

void player_reset_nb_key(t_player player) {
    assert(player);
    player->key = 0;
}

int player_get_curr_nb_bomb_set(t_player player) {
    assert(player);
    return player->curr_nb_bomb_set;
}

int player_get_total_nb_bomb_set(t_player player) {
    assert(player);
    return player->total_nb_bomb_set;
}

void player_inc_total_nb_bomb_set(t_player player) {
    assert(player);
    player->total_nb_bomb_set += 1;
}

void player_inc_curr_nb_bomb_set(t_player player) {
    assert(player);
    player->curr_nb_bomb_set += 1;
}

void player_dec_curr_nb_bomb_set(t_player player) {
    assert(player);
    player->curr_nb_bomb_set -= 1;
}

void player_inc_nb_bomb(t_player player) {
    assert(player);
    player->nb_bomb += 1;
}

void player_dec_nb_bomb(t_player player) {
    assert(player);
    player->nb_bomb -= 1;
}

void player_reset_bonus(t_player player) {
    player->nb_bomb = 3;
    player->curr_nb_bomb_set = 0;
    player->total_nb_bomb_set = 0;
    player->life = 2;
    player->power = BOMB_RANGE_DEFAULT;
    player->key = 0;
    player->finish = 0;

}

void player_from_map(t_player player, t_map map) {
    int i, j;
    for (i = 0; i < map_get_width(map); i++) {
        for (j = 0; j < map_get_height(map); j++) {
            if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
                player->x = i;
                player->y = j;
            }
        }
    }
}

static int player_move_aux(t_player player, t_map map, int x1, int y1, int x2,
        int y2) {

    if (!map_is_inside(map, x1, y1))
        return 0;


    switch (map_get_cell_type(map, x1, y1)) {
        case CELL_SCENERY:
            return 0;
            break;

        case CELL_CASE:
            if(player->current_way==SOUTH && map_get_cell_type(map, x1, y1+1)==CELL_EMPTY && y1+1<=11 ){
            	map_set_cell_type(map, x1, y1+1, CELL_CASE);

            }
            else if(player->current_way==NORTH && map_get_cell_type(map, x1, y1-1)==CELL_EMPTY && y1-1>=0 ){
                        	map_set_cell_type(map, x1, y1-1, CELL_CASE);

                        }
            else if(player->current_way==EAST&& map_get_cell_type(map, x1+1, y1)==CELL_EMPTY && x1+1<=11 ){
                                    	map_set_cell_type(map, x1+1, y1, CELL_CASE);

                                    }
            else if(player->current_way==WEST && map_get_cell_type(map, x1-1, y1)==CELL_EMPTY&& x1-1>=0 ){
                                    	map_set_cell_type(map, x1-1, y1, CELL_CASE);

                                    }
            else return 0;
            break;

        case CELL_BONUS:
            break;

        case CELL_GOAL:
                                        
            return MAP_INC;
            break;

        case CELL_MONSTER:
            break;

        case CELL_CLOSED_DOOR:
            if (player->key == 1) {
                map_set_cell_type(map, x1, y1, CELL_DOOR);
            }
            return 0;
            break;

        case CELL_PLAYER:
            break;

        case CELL_EMPTY:

            break;

        case CELL_KEY:
            player->key = 1;
            break;

            //supprimé CELL_STONE car CELL_SCENERY
        case CELL_DOOR:

            return MAP_INC;
            break;
            
        case CELL_BOMB:
            return 0;
            break;
            
        case CELL_BOMBERWOMAN:
            player->finish = 1;
            break;

        default:
            break;
    }

    // Player has moved
    return 1;
}

int player_move(t_player player, t_map map,t_game game) {
    int x = player->x;
    int y = player->y;
    
    int move = 0;
 

    switch (player->current_way) {
        case NORTH:
            if (player->y - 1 < 0) break;
           
            if(player->next_bomb==1) break;
           if (player_move_aux(player, map, x, y - 1, x, y - 2) == 1) {
                player_bonus(player, map, x, y - 1);
                player->y--;
                move = 1;
            } else if (player_move_aux(player, map, x, y - 1, x, y - 2) == MAP_INC) {
                move = MAP_INC;
            }


            break;

        case SOUTH:
            if (player->y + 1 > 11) break;
            if (player_move_aux(player, map, x, y + 1, x, y + 2) == 1) {
                player_bonus(player, map, x, y + 1);
                player->y++;
                move = 1;

            } else if (player_move_aux(player, map, x, y + 1, x, y + 2) == MAP_INC) {
                move = MAP_INC;
            }

            break;

        case WEST:
            if (player->x - 1 < 0) break;
            if (player_move_aux(player, map, x - 1, y, x - 2, y) == 1) {
                player_bonus(player, map, x - 1, y);
                player->x--;
                move = 1;
            } else if (player_move_aux(player, map, x - 1, y, x - 2, y) == MAP_INC) {
                move = MAP_INC;
            }

            break;

        case EAST:
            if (player->x + 1 > 11) break;
            if (player_move_aux(player, map, x + 1, y, x + 2, y) == 1) {
                player->x++;
                move = 1;
            } else if (player_move_aux(player, map, x + 1, y, x + 2, y) == MAP_INC) {
                move = MAP_INC;
            }

            break;
    }

    if (move) {
        if(map_get_cell_type(map,x,y)!=CELL_BOMB){
        map_set_cell_type(map, x, y, CELL_EMPTY);}
        map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
    }
    return move;
}

int player_check_finish(t_player player) {
    return player->finish;
}

void player_display(t_player player, SDL_Surface *screen) { //Sticks the player on the screen
    assert(player);
    assert(player->directed_img[player->current_way]);
    assert(screen);
    
    SDL_Rect place;
    
    place.x = player->x * SIZE_BLOC;
    place.y = player->y * SIZE_BLOC;

    SDL_BlitSurface(player->directed_img[player->current_way], NULL, screen,
            &place);
}
void player_reset_next_bomb(t_player player){
    player->next_bomb=0;
}
void player_bonus(t_player player, t_map map, int x, int y) { //manages what happens when a player steps on a bonus
    if (map_get_cell_type(map, x, y) == CELL_BONUS) {
        if (map_get_bonus_type(map, x, y) == BONUS_BOMB_NB_INC) {

            player->nb_bomb++;
        } else if (map_get_bonus_type(map, x, y) == BONUS_BOMB_NB_DEC) {
            if (player->nb_bomb > 1)player->nb_bomb--;
        } else if (map_get_bonus_type(map, x, y) == BONUS_BOMB_RANGE_INC) {
            if (player->power < BOMB_RANGE_MAX)player->power++;
        } else if (map_get_bonus_type(map, x, y) == BONUS_BOMB_RANGE_DEC) {
            if (player->power > 1)player->power--;
        } else if (map_get_bonus_type(map, x, y) == BONUS_LIFE) {
            player->life++;
        }
    }
}

int bomb_display(t_game game, t_player player, t_bomb bomb, SDL_Surface *screen, t_map map) { //manages the bombs

    int i;

    SDL_Rect place_explosionright[player->power];
    SDL_Rect place_explosionleft[player->power];
    SDL_Rect place_explosionup[player->power];
    SDL_Rect place_explosiondown[player->power];
    SDL_Rect place_explosioncenter;
      
    if (bomb->isset != 1) { //is the bomb set?
        return 0;
    }
    if (bomb->x != 700) {
        SDL_Rect bomb_place;

        bomb_place.x = bomb->x;
        bomb_place.y = bomb->y;
        SDL_BlitSurface(bomb->img_bomb, NULL, screen, &bomb_place);
    }
    if (bomb->timer >= 30 * (BOMB_LIFETIME) && bomb->timer <= 30 * (BOMB_LIFETIME) + 5) { //sticks the explosions
        for (i = 1; i <= player->power; i++) {
            place_explosionright[i].x = bomb->x + SIZE_BLOC*i;
            place_explosionright[i].y = bomb->y;
            place_explosioncenter.x = bomb->x;
            place_explosioncenter.y = bomb->y;
            place_explosionleft[i].x = bomb->x - SIZE_BLOC*i;
            place_explosionleft[i].y = bomb->y;
            place_explosionup[i].x = bomb->x;
            place_explosionup[i].y = bomb->y - SIZE_BLOC*i;
            place_explosiondown[i].x = bomb->x;
            place_explosiondown[i].y = bomb->y + SIZE_BLOC*i;
            SDL_BlitSurface(bomb->explosion, NULL, screen, &place_explosioncenter);
        }
        for (i = 1; i <= player->power; i++) {
            if (map_get_cell_type(map, place_explosionright[i].x / SIZE_BLOC, place_explosionright[i].y / SIZE_BLOC) != CELL_SCENERY) {
                SDL_BlitSurface(bomb->explosion, NULL, screen, &place_explosionright[i]);
            } else break;
        }
        for (i = 1; i <= player->power; i++) {
            if (map_get_cell_type(map, place_explosionleft[i].x / SIZE_BLOC, place_explosionleft[i].y / SIZE_BLOC) != CELL_SCENERY) {
                SDL_BlitSurface(bomb->explosion, NULL, screen, &place_explosionleft[i]);
            } else break;
        }
        for (i = 1; i <= player->power; i++) {
            if (map_get_cell_type(map, place_explosionup[i].x / SIZE_BLOC, place_explosionup[i].y / SIZE_BLOC) != CELL_SCENERY) {
                SDL_BlitSurface(bomb->explosion, NULL, screen, &place_explosionup[i]);
            } else break;
        }
        for (i = 1; i <= player->power; i++) {
            if (map_get_cell_type(map, place_explosiondown[i].x / SIZE_BLOC, place_explosiondown[i].y / SIZE_BLOC) != CELL_SCENERY) {
                SDL_BlitSurface(bomb->explosion, NULL, screen, &place_explosiondown[i]);
            } else break;
        }


        for (i = 1; i <= player->power; i++) {

            if (((player->x == place_explosionleft[i].x / (SIZE_BLOC) && player->y == place_explosionleft[i].y / (SIZE_BLOC)) //if bomberman is hit by the bomb
                    || (player->x == place_explosionright[i].x / (SIZE_BLOC) && player->y == place_explosionright[i].y / (SIZE_BLOC))
                    || (player->x == place_explosioncenter.x / (SIZE_BLOC) && player->y == place_explosioncenter.y / (SIZE_BLOC))
                    || (player->x == place_explosionup[i].x / (SIZE_BLOC) && player->y == place_explosionup[i].y / (SIZE_BLOC))
                    || (player->x == place_explosiondown[i].x / (SIZE_BLOC) && player->y == place_explosiondown[i].y / (SIZE_BLOC)))
                    && player->life != 0 && bomb->timer == 30 * (BOMB_LIFETIME)) {
                player->life--;
                break;
            }
        }
       
        //à placer dans une fonction d'ici
        for (i = 1; i <= player->power; i++) { //stops the explosion if it meets a scenery or explode a case
            if (map_get_cell_type(map, place_explosiondown[i].x / SIZE_BLOC, place_explosiondown[i].y / SIZE_BLOC) == CELL_SCENERY) {
                break;
                break;
            } else if (map_get_cell_type(map, place_explosiondown[i].x / SIZE_BLOC, place_explosiondown[i].y / SIZE_BLOC) == CELL_CASE) {
                map_set_cell_type(map, place_explosiondown[i].x / SIZE_BLOC, place_explosiondown[i].y / SIZE_BLOC, random_bonus(game, place_explosiondown[i].x / SIZE_BLOC, place_explosiondown[i].y / SIZE_BLOC));


            }
        }
        
        for (i = 1; i <= player->power; i++) {

            if (map_get_cell_type(map, place_explosionleft[i].x / SIZE_BLOC, place_explosionleft[i].y / SIZE_BLOC) == CELL_SCENERY) {
                break;
                break;
            } else if (map_get_cell_type(map, place_explosionleft[i].x / SIZE_BLOC, place_explosionleft[i].y / SIZE_BLOC) == CELL_CASE) {

                map_set_cell_type(map, place_explosionleft[i].x / SIZE_BLOC, place_explosionleft[i].y / SIZE_BLOC, random_bonus(game, place_explosionleft[i].x / SIZE_BLOC, place_explosionleft[i].y / SIZE_BLOC));

            }
        }
        for (i = 1; i <= player->power; i++) {

            if (map_get_cell_type(map, place_explosionright[i].x / SIZE_BLOC, place_explosionright[i].y / SIZE_BLOC) == CELL_SCENERY) {
                break;
                break;
            } else if (map_get_cell_type(map, place_explosionright[i].x / SIZE_BLOC, place_explosionright[i].y / SIZE_BLOC) == CELL_CASE) {
                map_set_cell_type(map, place_explosionright[i].x / SIZE_BLOC, place_explosionright[i].y / SIZE_BLOC, random_bonus(game, place_explosionright[i].x / SIZE_BLOC, place_explosionright[i].y / SIZE_BLOC));
            }
        }
        for (i = 1; i <= player->power; i++) {

            if (map_get_cell_type(map, place_explosionup[i].x / SIZE_BLOC, place_explosionup[i].y / SIZE_BLOC) == CELL_SCENERY) {
                break;
                break;
            } else if (map_get_cell_type(map, place_explosionup[i].x / SIZE_BLOC, place_explosionup[i].y / SIZE_BLOC) == CELL_CASE) {
                map_set_cell_type(map, place_explosionup[i].x / SIZE_BLOC, place_explosionup[i].y / SIZE_BLOC, random_bonus(game, place_explosionup[i].x / SIZE_BLOC, place_explosionup[i].y / SIZE_BLOC));
            }
        }
        // jusque là
    }
    if (bomb->timer == 30 * (BOMB_LIFETIME) + 5) {
        
        map_set_cell_type(map,bomb->x/SIZE_BLOC,bomb->y/SIZE_BLOC,CELL_EMPTY);
        bomb->x = 700; //à defaut d'une suppression de la bombe, on la sort du champ
        bomb->y = 700;
        player_dec_curr_nb_bomb_set(player);
        for (i = 1; i <= player->power; i++) {
            place_explosionright[i].y = 700;
            place_explosionleft[i].y = 700;
            place_explosionup[i].y = 700;
            place_explosiondown[i].y = 700;
            place_explosioncenter.y = 700;

        }
    }


    return 0;

}

int get_life(t_player player) {
    return player->life;
}

void player_dec_life(t_player player) {
    player->life -= 1;
}

void player_set_life(t_player player, int nb_life) {
    player->life = nb_life;
}

int get_player_power(t_player player) {
    return player->power;
}

void player_set_power(t_player player, int num) {
    assert(player);
    player->power = num;
}


