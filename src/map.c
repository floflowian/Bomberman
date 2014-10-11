/*
 * map.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent Reveillere, Vincent Filou
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#include <time.h>
#include "../include/constant.h"
#include "../include/map.h"
#include "../include/misc.h"
#include "../include/level.h"
#include "../include/game.h"

typedef char t_cell_type_;

struct s_map {
    int width;
    int height;
    int num_map;
    t_cell_type_* grid;

    SDL_Surface* img_box;
    SDL_Surface* img_goal;
    SDL_Surface* img_bonus_bomb_range_inc;
    SDL_Surface* img_bonus_bomb_range_dec;
    SDL_Surface* img_bonus_bomb_nb_inc;
    SDL_Surface* img_bonus_bomb_nb_dec;
    SDL_Surface* img_bonus_life;
    SDL_Surface* img_key;
    SDL_Surface* img_door;
    SDL_Surface* img_closed_door;
    SDL_Surface* img_stone;
    SDL_Surface* img_tree;
    SDL_Surface* img_bomb;
    SDL_Surface* img_bomberwoman;
};

struct s_tab {
    int x, y, value_sprite, empty,player_set;
    SDL_Surface * sprite_type;

};

#define CELL(i,j) (i +  map->width * j)

t_map map_new(int width, int height) { //creates a new map object 
    assert(width > 0 && height > 0);

    t_map map = malloc(sizeof (struct s_map));
    if (map == NULL)
        exit(EXIT_FAILURE);

    map->width = width;
    map->height = height;
    map->num_map = 1;
    map->grid = (t_cell_type_ *) malloc(height * width * sizeof (t_cell_type_));
    if (map->grid == NULL)
        exit(EXIT_FAILURE);

    // Grid cleaning
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            map->grid[CELL(i, j)] = CELL_EMPTY;
        }
    }

    // Sprite loading
    map->img_bomb = load_image(IMG_BOMB_TTL4);
    map->img_tree = load_image(IMG_MAP_TREE);
    map->img_box = load_image(IMG_MAP_CASE);
    map->img_goal = load_image(IMG_MAP_GOAL);
    map->img_key = load_image(IMG_MAP_KEY);
    map->img_stone = load_image(IMG_MAP_STONE);
    map->img_door = load_image(IMG_MAP_DOOR);
    map->img_closed_door = load_image(IMG_MAP_CLOSED_DOOR);
    map->img_bonus_bomb_range_inc = load_image(IMG_BONUS_BOMB_RANGE_INC);
    map->img_bonus_bomb_range_dec = load_image(IMG_BONUS_BOMB_RANGE_DEC);
    map->img_bonus_bomb_nb_inc = load_image(IMG_BONUS_BOMB_NB_INC);
    map->img_bonus_bomb_nb_dec = load_image(IMG_BONUS_BOMB_NB_DEC);
    map->img_bonus_life = load_image(IMG_BONUS_LIFE);
    map->img_bomberwoman = load_image(IMG_BOMBERWOMAN);
    return map;
}

int map_is_inside(t_map map, int x, int y) {
    assert(map);
    // TODO ...
    return 1;
}

void map_free(t_map map) {
    if (map == NULL) {
        return;
    }

    free(map->grid);

    SDL_FreeSurface(map->img_bonus_bomb_nb_dec);
    SDL_FreeSurface(map->img_bonus_bomb_nb_inc);
    SDL_FreeSurface(map->img_bonus_bomb_range_dec);
    SDL_FreeSurface(map->img_bonus_bomb_range_inc);
    SDL_FreeSurface(map->img_bonus_life);
    SDL_FreeSurface(map->img_closed_door);
    SDL_FreeSurface(map->img_door);
    SDL_FreeSurface(map->img_key);
    SDL_FreeSurface(map->img_stone);
    SDL_FreeSurface(map->img_box);
    SDL_FreeSurface(map->img_goal);
    SDL_FreeSurface(map->img_tree);
    SDL_FreeSurface(map->img_bomberwoman);

    free(map);
}

int map_get_width(t_map map) {
    assert(map != NULL);
    return map->width;
}

int map_get_height(t_map map) {
    assert(map != NULL);
    return map->height;
}

t_cell_type map_get_cell_type(t_map map, int x, int y) {
    assert(map && map_is_inside(map, x, y));
    return map->grid[CELL(x, y)] & 15;
}

t_bonus_type map_get_bonus_type(t_map map, int x, int y) {
    assert(map && map_is_inside(map, x, y));
    return map->grid[CELL(x, y)] >> 4;
}

void map_set_cell_type(t_map map, int x, int y, t_cell_type type) {
    assert(map && map_is_inside(map, x, y));
    map->grid[CELL(x, y)] = type;
}

void display_bonus(t_map map, SDL_Surface* screen, SDL_Rect* place,
        t_bonus_type type) { //changes t_cell_type to t_bonus_type

    // bonus is encoded with the 4 most significant bits
    switch (type >> 4) { // changed type >> 4 to type
        case BONUS_BOMB_RANGE_INC:

            SDL_BlitSurface(map->img_bonus_bomb_range_inc, NULL, screen, place);
            break;

        case BONUS_BOMB_RANGE_DEC:

            SDL_BlitSurface(map->img_bonus_bomb_range_dec, NULL, screen, place);
            break;

        case BONUS_BOMB_NB_INC:
            SDL_BlitSurface(map->img_bonus_bomb_nb_inc, NULL, screen, place);
            break;

        case BONUS_BOMB_NB_DEC:
            SDL_BlitSurface(map->img_bonus_bomb_nb_dec, NULL, screen, place);
            break;
        case BONUS_LIFE:
            SDL_BlitSurface(map->img_bonus_life, NULL, screen, place);
            break;


    }


}

void display_scenery(t_map map, SDL_Surface* screen, SDL_Rect* place,
        t_cell_type_ type) {

    switch (type >> 4) {
        case SCENERY_STONE:
            SDL_BlitSurface(map->img_stone, NULL, screen, place);
            break;

        case SCENERY_TREE:
            SDL_BlitSurface(map->img_tree, NULL, screen, place);
            break;
    }
}

void map_display(t_map map, SDL_Surface *screen) {
    int i, j;

    assert(map != NULL);
    assert(screen != NULL);
    assert(map->height > 0 && map->width > 0);

    assert(map->img_bonus_bomb_nb_dec != NULL);
    assert(map->img_bonus_bomb_nb_inc != NULL);
    assert(map->img_bonus_bomb_range_dec != NULL);
    assert(map->img_bonus_bomb_range_inc != NULL);
    assert(map->img_closed_door != NULL);
    assert(map->img_door != NULL);
    assert(map->img_key != NULL);
    assert(map->img_stone != NULL);
    assert(map->img_box != NULL);
    assert(map->img_goal != NULL);
    assert(map->img_bomberwoman != NULL);

    for (i = 0; i < map->width; i++) {
        for (j = 0; j < map->height; j++) {
            SDL_Rect place;
            place.x = i * SIZE_BLOC;
            place.y = j * SIZE_BLOC;

            t_cell_type_ type = map->grid[CELL(i, j)];

            switch (type & 15) { // type is encoded with 4 bits (the & 15 (1111)) is a mask to keep the four less significant bits)
                case CELL_GOAL:
                    SDL_BlitSurface(map->img_goal, NULL, screen, &place);
                    break;
                case CELL_SCENERY:
                    display_scenery(map, screen, &place, type);
                    break;
                case CELL_CASE:
                    SDL_BlitSurface(map->img_box, NULL, screen, &place);
                    break;
                case CELL_BONUS:
                    display_bonus(map, screen, &place, type);
                    break;
                case CELL_KEY:
                    SDL_BlitSurface(map->img_key, NULL, screen, &place);
                    break;
                case CELL_DOOR:
                    SDL_BlitSurface(map->img_door, NULL, screen, &place);
                    break;
                case CELL_CLOSED_DOOR:
                    SDL_BlitSurface(map->img_closed_door, NULL, screen, &place);
                    break;
                case CELL_BOMBERWOMAN:
                    SDL_BlitSurface(map->img_bomberwoman, NULL, screen, &place);
                    break;

            }
        }

    }
}

void map_set_grid(t_map map, int i, int themap) {
    map->grid[i] = themap;
}

t_map map_get_default(t_level level) { //reads the default maps

    int width, height;

    int i;
    FILE* fichier = NULL;
    char pathmap[80];
    sprintf(pathmap, "map/%d/map_c_%d.txt", level_get_num_level(level), level_get_num_map(level) + 1); //sets the path of the file regarding the current map/level
    fichier = fopen(pathmap, "r");
    if (fichier != NULL) {
        if (fscanf(fichier, "%d:%d", &width, &height) != 2) {
            printf("Failed to read Width & Height");
        }

    }

    //
    t_map map = map_new(width, height);



    t_cell_type_ themap[144];
    int tab[144];
    for (i = 0; i < width * height; i++) {  //reads the file and stores the values in the map object
        if (fscanf(fichier, "%d ", &tab[i]) == 1) {
            themap[i]=tab[i];
            if (themap[i] == 21) {
                themap[i] = 2 | (1 << 4);
            } else if (themap[i] == 22) {
                themap[i] = 2 | (2 << 4);
            }
        }
        else themap[i]=0;
    }

    fclose(fichier);

    for (int i = 0; i < 144; i++) {
        map->grid[i] = themap[i];
    }
    return map;
}

int map_editor(SDL_Surface *screen) {
    int i, j;

    FILE* fichier = NULL;
    fichier = fopen("map/map_default.txt", "w+"); //opens the target file that will contain the custom map
    fprintf(fichier, "12:12\n");
    int placecursor = 5;
    int valuecursor = 1;
    int current_cell = 1;
    screen_editor(screen, placecursor);
    SDL_Event event;
    int keep = 1;
    t_tab tab = malloc(sizeof (struct s_tab));
    SDL_Rect place[100];
    SDL_Surface * cell[144] = {CELL_EMPTY}; //creates as much surfaces as possible
    t_cell_type grid[12][12];   //the grid will be used to write the cell types in the file
    for (i = 0; i <= 11; i++) {
        for (j = 0; j <= 11; j++) {
            grid[i][j] = 0;
        }
    }
    for (i = 0; i <= 100; i++) { //each surface will be firstly located in (0,0)
        place[i].x = 0;
        place[i].y = 0;
    }
    tab->x = 0;
    tab->y = 0;
    tab->sprite_type = NULL;
    tab->empty = 0;
    tab->player_set=0;

    int white_bloc = ((12 * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
    while (keep != 0) {
        tab->empty = 0;
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
        screen_editor(screen, placecursor);

        if (current_cell > 1) { //Sticks all the cells the user has defined
            for (i = 1; i <= current_cell - 1; i++) {

                SDL_BlitSurface(cell[i], NULL, screen, &place[i]);

            }
        }
        SDL_Flip(screen);
        SDL_WaitEvent(&event);

        switch (event.type) {


            case SDL_QUIT:
                return 2;
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    case SDLK_RIGHT: //move the cursor that chooses which cell type to stick
                        if (placecursor < 10 * SIZE_BLOC) {
                            placecursor = placecursor + 0.25 * white_bloc + SIZE_BLOC;
                            valuecursor++;
                        }

                        break;
                    case SDLK_LEFT:
                        if (placecursor > 5) {
                            valuecursor--;
                            placecursor = placecursor - 0.25 * white_bloc - SIZE_BLOC;
                        }

                        break;
                    case SDLK_RETURN: //validates the choice of a cell type

                        if (valuecursor == 1) {
                            tab->sprite_type = IMG_Load(IMG_MAP_STONE);
                            tab->value_sprite = 21;
                        } else if (valuecursor == 2) {
                            tab->sprite_type = IMG_Load(IMG_MAP_TREE);
                            tab->value_sprite = 22;
                        } else if (valuecursor == 3) {
                            tab->sprite_type = IMG_Load(IMG_MAP_KEY);
                            tab->value_sprite = 8;
                        } else if (valuecursor == 4) {
                            tab->sprite_type = IMG_Load(IMG_MAP_CASE);
                            tab->value_sprite = 4;
                        } else if (valuecursor == 5) {
                            tab->sprite_type = IMG_Load(IMG_MAP_CLOSED_DOOR);
                            tab->value_sprite = 10;
                        } else if (valuecursor == 6) {
                            tab->sprite_type = IMG_Load(IMG_MONSTER_DOWN);
                            tab->value_sprite = 6;
                        } else if (valuecursor == 7) {
                            tab->sprite_type = IMG_Load(IMG_PLAYER_DOWN);
                            tab->value_sprite = 3;
                            if(tab->player_set==1){break;} //check if a player is already placed
                            tab->player_set=1;
                        } else if (valuecursor == 7) {
                            tab->sprite_type = IMG_Load(IMG_PLAYER_DOWN);
                            tab->value_sprite = 3;

                        } else if (valuecursor == 8) {
                            tab->sprite_type = IMG_Load(IMG_ERASER);
                            tab->value_sprite = 0;
                            tab->empty = 1;
                        }
                        SDL_BlitSurface(tab->sprite_type, NULL, screen, &place[current_cell]);
                        SDL_Flip(screen);

                        move_cell(tab->x, tab->y, screen, tab->sprite_type, tab, placecursor, place, cell, current_cell);
                        place[current_cell].x = tab->x;
                        place[current_cell].y = tab->y;

                        if (tab->empty == 1) { //if the user wants to correct something he did wrong
                            for (i = 1; i <= current_cell - 1; i++) {
                                if (place[i].x == tab->x && place[i].y == tab->y) {
                                    cell[i] = CELL_EMPTY;
                                }
                                if(grid[place[i].x/SIZE_BLOC][place[i].y/SIZE_BLOC]==3){tab->player_set=0;}
                            }
                        } else { //or he defines a currently empty case or replaces an existing case
                            for (i = 1; i <= current_cell - 1; i++) {
                                if (place[i].x == tab->x && place[i].y == tab->y && cell[i] != CELL_EMPTY) {
                                    cell[i] = CELL_EMPTY;
                                }
                            }
                            cell[current_cell] = tab->sprite_type;
                        }
                        grid[tab->x / SIZE_BLOC][tab->y / SIZE_BLOC] = tab->value_sprite; //sets the grid cell
                        current_cell++;
                        for (i = 1; i <= current_cell - 1; i++) {
                            SDL_BlitSurface(cell[i], NULL, screen, &place[i]);
                        }
                        tab->x = 0;
                        tab->y = 0;
                        SDL_Flip(screen);


                        break;
                    case SDLK_d: // the user is done

                        for (i = 0; i <= 11; i++) { //write it all in the file
                            for (j = 0; j <= 11; j++) {
                                fprintf(fichier, "%d", grid[j][i]);
                                if (j != 11) {
                                    fprintf(fichier, " ");
                                }
                            }
                            fprintf(fichier, "\n");
                        }
                        keep = 0;

                        break;


                    default:
                        screen_editor(screen, placecursor);
                        break;
                }

                break;


        }

    }
    fclose(fichier);
    return 0;
}

void screen_editor(SDL_Surface* screen, int theplacecursor) { //sticks the frame of the editor
    int i;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_Surface *spritestone = NULL;
    SDL_Rect place;
    SDL_Rect placecursor;
    SDL_Surface *spritetree = NULL;
    SDL_Surface *spritekey = NULL;
    SDL_Surface *spritecase = NULL;
    SDL_Surface *spritedoor = NULL;
    SDL_Surface *spriteplayer = NULL;
    SDL_Surface *spritemonster = NULL;
    SDL_Surface * spriteeraser = NULL;
    SDL_Surface *line = NULL;
    SDL_Surface *cursor = NULL;
    cursor = IMG_Load(IMG_CURSOR);
    line = IMG_Load(IMG_BANNER_LINE);
    spritestone = IMG_Load(IMG_MAP_STONE);
    spritetree = IMG_Load(IMG_MAP_TREE);
    spritekey = IMG_Load(IMG_MAP_KEY);
    spritecase = IMG_Load(IMG_MAP_CASE);
    spritedoor = IMG_Load(IMG_MAP_CLOSED_DOOR);
    spritemonster = IMG_Load(IMG_MONSTER_DOWN);
    spriteplayer = IMG_Load(IMG_PLAYER_DOWN);
    spriteeraser = IMG_Load(IMG_ERASER);
    place.y = 12 * SIZE_BLOC;
    for (i = 0; i < 12; i++) {
        place.x = i * SIZE_BLOC;
        SDL_BlitSurface(line, NULL, screen, &place);
    }

    place.y = (12 * SIZE_BLOC) + SIZE_LINE;

    int white_bloc = ((12 * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;

    place.x = 0;
    SDL_BlitSurface(spritestone, NULL, screen, &place);
    placecursor.x = theplacecursor;
    placecursor.y = 12 * SIZE_BLOC -25 ;
    SDL_BlitSurface(cursor, NULL, screen, &placecursor);
    // TODO : link lifes in banner and lifes of player
    place.x = 0.25 * white_bloc + 1 * SIZE_BLOC;
    SDL_BlitSurface(spritetree, NULL, screen, &place);

    place.x = 0.5 * white_bloc + 2 * SIZE_BLOC;
    SDL_BlitSurface(spritekey, NULL, screen, &place);

    place.x = 0.75 * white_bloc + 3 * SIZE_BLOC;
    SDL_BlitSurface(spritecase, NULL, screen, &place);

    place.x = 1 * white_bloc + 4 * SIZE_BLOC;
    SDL_BlitSurface(spritedoor, NULL, screen, &place);
    place.x = 1.25 * white_bloc + 5 * SIZE_BLOC;
    SDL_BlitSurface(spritemonster, NULL, screen, &place);
    place.x = 1.5 * white_bloc + 6 * SIZE_BLOC;
    SDL_BlitSurface(spriteplayer, NULL, screen, &place);
    place.x = 1.75 * white_bloc + 7 * SIZE_BLOC;
    SDL_BlitSurface(spriteeraser, NULL, screen, &place);
    SDL_Flip(screen);

}

void move_cell(int place_x, int place_y, SDL_Surface * screen, SDL_Surface * sprite, t_tab tab, int theplacecursor, SDL_Rect totalplace[100], SDL_Surface * cell[100], int current_cell) {
    SDL_Event event;
    int keep = 1;
    int i;
    SDL_Rect place;
    place.x = place_x;
    place.y = place_y;
    while (keep) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                SDL_Quit();
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {

                    case SDLK_RIGHT:
                        if (place.x < 11 * SIZE_BLOC) {
                            //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
                            screen_editor(screen, theplacecursor);
                            place.x = place.x + SIZE_BLOC;

                            SDL_BlitSurface(sprite, NULL, screen, &place);
                            if (current_cell != 1) {
                                for (i = 1; i <= current_cell - 1; i++) {

                                    SDL_BlitSurface(cell[i], NULL, screen, &totalplace[i]);

                                }
                            }
                            SDL_Flip(screen);
                        }


                        break;
                    case SDLK_LEFT:
                        if (place.x > 0) {
                            //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
                            screen_editor(screen, theplacecursor);
                            place.x = place.x - SIZE_BLOC;
                            SDL_BlitSurface(sprite, NULL, screen, &place);
                            for (i = 1; i <= current_cell - 1; i++) {

                                SDL_BlitSurface(cell[i], NULL, screen, &totalplace[i]);

                            }
                            SDL_Flip(screen);
                        }

                        break;
                    case SDLK_UP:
                        if (place.y > 0) {
                            //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
                            screen_editor(screen, theplacecursor);
                            place.y = place.y - SIZE_BLOC;
                            SDL_BlitSurface(sprite, NULL, screen, &place);
                            for (i = 1; i <= current_cell - 1; i++) {

                                SDL_BlitSurface(cell[i], NULL, screen, &totalplace[i]);

                            }
                            SDL_Flip(screen);
                        }

                        break;
                    case SDLK_DOWN:
                        if (place.y < 10 * SIZE_BLOC) {
                            //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
                            screen_editor(screen, theplacecursor);
                            place.y = place.y + SIZE_BLOC;
                            SDL_BlitSurface(sprite, NULL, screen, &place);
                            for (i = 1; i <= current_cell - 1; i++) {

                                SDL_BlitSurface(cell[i], NULL, screen, &totalplace[i]);

                            }
                            SDL_Flip(screen);
                        }

                        break;
                    case SDLK_RETURN:

                        keep = 0;
                        break;
                    default:
                        break;
                }
        }
    }
    tab->x = place.x;
    tab->y = place.y;
}
