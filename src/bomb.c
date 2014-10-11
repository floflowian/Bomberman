/*
 * bomb.c
 *
 *  Created on: 26 févr. 2013
 *      Author: flouistherese
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../include/game.h"
#include "../include/player.h"
#include "../include/bomb.h"

struct s_player {
    int x, y, life;
    SDL_Surface * directed_img[4];
    enum e_way current_way;
    int nb_bomb;
    int power;
};

struct s_bomb {
    int x, y, timer, power, isset;
    SDL_Surface * img_bomb;
    SDL_Surface * explosion;
};

t_bomb bomb_init() {
    t_bomb bomb = malloc(sizeof (*bomb));

    if (!bomb)
        error("Memory error");

    return bomb;
}

void bomb_free(t_bomb bomb) {
}
int bomb_get_x(t_bomb bomb){ 
    return bomb->x;
}
int bomb_get_y(t_bomb bomb){
    
    return bomb->y;
}
void bomb_set_timer(t_bomb bomb) {
    if (bomb->timer == 30 * (BOMB_LIFETIME - 3)) {
        bomb->img_bomb = load_image(IMG_BOMB_TTL3);
        bomb->timer++;
    } else if (bomb->timer == 30 * (BOMB_LIFETIME - 2)) {
        bomb->img_bomb = load_image(IMG_BOMB_TTL2);
        bomb->timer++;
    } else if (bomb->timer == 30 * (BOMB_LIFETIME - 1)) {
        bomb->img_bomb = load_image(IMG_BOMB_TTL1);
        bomb->timer++;
    }        
    else bomb->timer++;


}

void bomb_set_place(t_bomb bomb, t_player player) {
    bomb->explosion = load_image(IMG_BOMB_EXP);
    bomb->img_bomb = load_image(IMG_BOMB_TTL4);
    bomb->x = player->x * SIZE_BLOC;
    bomb->y = player->y * SIZE_BLOC;
    bomb->timer = 1;
    bomb->isset = 1;

}
