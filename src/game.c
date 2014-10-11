/*
 * game.c
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#include <assert.h>
#include <time.h>

#include "../include/game.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/misc.h"
#include "../include/level.h"
#include "../include/bomb.h"
#include "../include/monster.h"

struct s_game {  //structure containing all the information about the game
    t_level curr_level; 
    int level;
    t_player player;
    t_monster monster[10];
    t_bomb bomb[20];
    int pause;
    int start;
    int autorisation_dec[10];
    SDL_Surface * number[10];
    SDL_Surface * banner_life;
    SDL_Surface * banner_bomb;
    SDL_Surface * banner_range;
    SDL_Surface * banner_line;
    SDL_Surface * banner_key;
    SDL_Surface * banner_level;
    int nb_monster;
};

static struct s_game the_game;

t_game game_new(int choice) { // init. of the game depending on Loading save or new game
    srand(time(NULL));
    int i, x, y, nb_bomb, nb_key, nb_life, power, width, height, num_level, num_map;
    FILE* fichier = NULL;

    fichier = fopen("save.txt", "r");  //open the save file
    //extract all the information
    if(fscanf(fichier, "%d:%d:%d:%d:%d:%d", &x, &y, &nb_bomb, &nb_key, &nb_life, &power)!=6) printf("Failed to read saved player information");
    if (fichier != NULL) {
        if (fscanf(fichier, "%d:%d", &num_level, &num_map) != 2) {
            printf("Failed to read Num Level/Map");
        }
        if (fscanf(fichier, "%d:%d", &width, &height) != 2) {
            printf("Failed to read Width & Height");
        }

    }
    t_map map = map_new(width, height);
    int themap[144];
    for (i = 0; i < width * height; i++) { //Read the saved map 
        if (fscanf(fichier, "%d ", &themap[i]) == 1) {
            if (themap[i] == 21) {
                themap[i] = 2 | (1 << 4);
            } else if (themap[i] == 22) {
                themap[i] = 2 | (2 << 4);
            } else if (themap[i] == 51) {
                themap[i] = 5 | (1 << 4);
            } else if (themap[i] == 52) {
                themap[i] = 5 | (2 << 4);
            } else if (themap[i] == 53) {
                themap[i] = 5 | (3 << 4);
            } else if (themap[i] == 54) {
                themap[i] = 5 | (4 << 4);
            } else if (themap[i] == 55) {
                themap[i] = 5 | (5 << 4);
            } else if (themap[i] == 56) {
                themap[i] = 5 | (6 << 4);
            }

        }
    }

    fclose(fichier);

    for (i = 0; i < 144; i++) {
        map_set_grid(map, i, themap[i]);
    }


    if (choice == 1) { //load the information about the player & level
        
        the_game.curr_level = level_get_level(1, map, num_level, num_map);
        t_player player = player_init();
        
        player_set_nb_bomb(player, nb_bomb);
        player_set_nb_key(player, nb_key);
        player_set_life(player, nb_life);
        player_set_power(player, power);

        the_game.player = player;


    } 
    else { // or create a new player & level
        the_game.curr_level = level_get_level(0, map, num_level, num_map);
        the_game.player = player_init();
    } // get maps of the first level

    //create a set of 7 bombs
    the_game.bomb[0] = bomb_init();
    the_game.bomb[1] = bomb_init();
    the_game.bomb[2] = bomb_init();
    the_game.bomb[3] = bomb_init();
    the_game.bomb[4] = bomb_init();
    the_game.bomb[5] = bomb_init();
    the_game.bomb[6] = bomb_init();
    the_game.bomb[7] = bomb_init();
    the_game.bomb[8] = bomb_init();
    the_game.bomb[9] = bomb_init();
    the_game.bomb[10] = bomb_init();
    the_game.bomb[11] = bomb_init();
    the_game.bomb[12] = bomb_init();
     the_game.bomb[13] = bomb_init();
    the_game.bomb[14] = bomb_init();
    the_game.bomb[15] = bomb_init();
 
 
    the_game.nb_monster = 0;
    //these will be useful to limit the decrementation of the player's life
    the_game.autorisation_dec[0] = 1;
    the_game.autorisation_dec[1] = 1;
    the_game.autorisation_dec[2] = 1;
    the_game.autorisation_dec[3] = 1;
    the_game.autorisation_dec[4] = 1;
    the_game.autorisation_dec[5] = 1;
    the_game.autorisation_dec[6] = 1;
    the_game.autorisation_dec[7] = 1;
    the_game.autorisation_dec[8] = 1;
    the_game.autorisation_dec[9] = 1;
    the_game.autorisation_dec[10] = 1;

    the_game.pause = 0;
    the_game.start = 1;
        player_from_map(the_game.player, level_get_curr_map(the_game.curr_level));
    //player_from_map(the_game.player, level_get_map(the_game.curr_level, 0)); // get x,y of the player on the first map
        //level_get_curr_map(game_get_curr_level(game))
    /* load banner's imgs in memory */
    the_game.number[0] = load_image(IMG_BANNER_0);
    the_game.number[1] = load_image(IMG_BANNER_1);
    the_game.number[2] = load_image(IMG_BANNER_2);
    the_game.number[3] = load_image(IMG_BANNER_3);
    the_game.number[4] = load_image(IMG_BANNER_4);
    the_game.number[5] = load_image(IMG_BANNER_5);
    the_game.number[6] = load_image(IMG_BANNER_6);
    the_game.number[7] = load_image(IMG_BANNER_7);
    the_game.number[8] = load_image(IMG_BANNER_8);
    the_game.number[9] = load_image(IMG_BANNER_9);
    the_game.banner_life = load_image(IMG_BANNER_LIFE);
    the_game.banner_bomb = load_image(IMG_BANNER_BOMB);
    the_game.banner_range = load_image(IMG_BANNER_RANGE);
    the_game.banner_line = load_image(IMG_BANNER_LINE);
    the_game.banner_key = load_image(IMG_BANNER_KEY);
    the_game.banner_level = load_image(IMG_BANNER_LEVEL);

    return &the_game;
}

void game_free(t_game game) {
    assert(game);

    player_free(game->player);
    level_free(game->curr_level);
}

void game_inc_nb_monster(t_game game) {
    game->nb_monster++;
}

t_player game_get_player(t_game game) {
    assert(game);
    return game->player;
}

t_monster game_get_monster(t_game game, int j) {
    assert(game);
    return game->monster[j];
}

t_bomb game_get_bomb(t_game game) {
    assert(game);
    int i = player_get_curr_nb_bomb_set(game->player);
    return game->bomb[i];
}
t_bomb game_get_bomb_i(t_game game,int i){
    return game->bomb[i];
}

void game_banner_display(t_game game, SDL_Surface* screen) {
    assert(game);
    assert(screen);
    t_player player = game->player;
    t_map map = level_get_curr_map(game_get_curr_level(game));
    //                t_level level=game_get_curr_level(game);

    SDL_Rect place;

    int i;
    place.y = (map_get_height(map)) * SIZE_BLOC;
    for (i = 0; i < map_get_width(map); i++) {
        place.x = i * SIZE_BLOC;
        SDL_BlitSurface(game->banner_line, NULL, screen, &place);
    }

    place.y = (map_get_height(map) * SIZE_BLOC) + SIZE_LINE;

    int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;

    place.x = 0;
    SDL_BlitSurface(game->banner_life, NULL, screen, &place);
    place.x = 0 + SIZE_BLOC;
    // TODO : link lifes in banner and lifes of player
    int life = get_life(game->player);
    SDL_BlitSurface(game->number[life], NULL, screen, &place);

    place.x = 1 * white_bloc + 1 * SIZE_BLOC;
    SDL_BlitSurface(game->banner_bomb, NULL, screen, &place);
    place.x = 1 * white_bloc + 2 * SIZE_BLOC;
    ;
    if (player_get_nb_bomb(game_get_player(game)) >= 0
            && player_get_nb_bomb(game_get_player(game)) < 10)
        SDL_BlitSurface(game->number[player_get_nb_bomb(game_get_player(game))],
            NULL, screen, &place);
    //

    place.x = 2 * white_bloc + 2 * SIZE_BLOC;
    SDL_BlitSurface(game->banner_range, NULL, screen, &place);
    place.x = 2 * white_bloc + 3 * SIZE_BLOC;
    // TODO : link bomb_range in banner and bomb_range of player
    int bomb_range = get_player_power(player);
    SDL_BlitSurface(game->number[bomb_range], NULL, screen, &place);

    place.x = 3 * white_bloc + 3 * SIZE_BLOC;
    SDL_BlitSurface(game->banner_key, NULL, screen, &place);
    place.x = 3 * white_bloc + 4 * SIZE_BLOC;
    // TODO : link bomb_range in banner and bomb_range of player	
    SDL_BlitSurface(game->number[player_get_nb_key(player)], NULL, screen, &place);

    //game->nb_monster
    place.x = 4 * white_bloc + 4 * SIZE_BLOC;
    SDL_BlitSurface(game->banner_level, NULL, screen, &place);
    place.x = 4 * white_bloc + 5 * SIZE_BLOC;
    // TODO : link bomb_range in banner and bomb_range of player	
    SDL_BlitSurface(game->number[level_get_num_level(game->curr_level)], NULL, screen, &place);
}

t_level game_get_curr_level(t_game game) {
    return game->curr_level;
}

int game_get_curr_num_level(t_game game) {
    return game->level;
}

uint input_update(t_game game, SDL_Surface* screen) {  //check the user's move and treat
    SDL_Event event;
    //        int i;
    t_player player = game_get_player(game);
    t_map map = level_get_curr_map(game_get_curr_level(game));
    t_level level = game_get_curr_level(game);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 1;
                    case SDLK_UP:
                        player_set_current_way(player, NORTH);
                        //player_move(player, map);
                        if (player_move(player, map,game) == MAP_INC) { //player on a closed door or level-flag, pass to the next map/level
                            
                            if (level_get_num_map(level) == 1) { //level done
                                splashscreen(screen, 3);
                                game->start = 1;
                                game->nb_monster = 0;
                                player_reset_bonus(player);
                                level_inc(level);
                                level_reset_cur_map(level);
                                player_set_x(player, 0);
                                player_set_y(player, 0);
                            } else {    //level undone, next map
                                level_map_inc(level);
                                player_reset_nb_key(player);
                                player_set_x(player, 0);
                                game->start = 1;
                                game->nb_monster = 0;
                                player_set_y(player, 0);
                                
                            }
                        }
                        return 0;
                        break;
                    case SDLK_DOWN:
                        player_set_current_way(player, SOUTH);
                        //player_move(player, map);
                        if (player_move(player, map,game)  == MAP_INC) {
                            if (level_get_num_map(level) == 1) {
                                splashscreen(screen, 3);
                                level_inc(level);
                                player_reset_bonus(player);
                                game->start = 1;
                                game->nb_monster = 0;
                                level_reset_cur_map(level);
                                player_set_x(player, 0);
                                player_set_y(player, 0);
                            } else {
                                
                                level_map_inc(level);
                                player_reset_nb_key(player);
                                game->start = 1;
                                game->nb_monster = 0;
                                player_set_x(player, 0);
                                player_set_y(player, 0);
                            }
                        }
                        return 0;
                        break;
                    case SDLK_RIGHT:
                        player_set_current_way(player, EAST);
                        //player_move(player, map);
                        if (player_move(player, map,game) == MAP_INC) {
                            if (level_get_num_map(level) == 1) {
                                splashscreen(screen, 3);
                                level_inc(level);
                                player_reset_bonus(player);
                                game->start = 1;
                                game->nb_monster = 0;
                                level_reset_cur_map(level);
                                player_set_x(player, 0);
                                player_set_y(player, 0);
                            } else {
                                level_map_inc(level);
                                player_reset_nb_key(player);
                                player_set_x(player, 0);
                                game->start = 1;
                                game->nb_monster = 0;
                                player_set_y(player, 0);
                            }
                        }

                        return 0;
                        break;
                    case SDLK_LEFT:
                        player_set_current_way(player, WEST);
                        //player_move(player, map);
                        if (player_move(player, map,game)  == MAP_INC) {
                            if (level_get_num_map(level) == 1) {
                                splashscreen(screen, 3);
                                level_inc(level);
                                player_reset_bonus(player);
                                game->start = 1;
                                game->nb_monster = 0;
                                level_reset_cur_map(level);
                                player_set_x(player, 0);
                                player_set_y(player, 0);
                            } else {
                                level_map_inc(level);
                                player_reset_nb_key(player);
                                game->start = 1;
                                game->nb_monster = 0;
                                player_set_x(player, 0);
                                player_set_y(player, 0);
                            }
                        }
                        return 0;
                        break;
                    case SDLK_SPACE:

                        if (player_get_curr_nb_bomb_set(player) < player_get_nb_bomb(player)) { //if the player still has some bombs left
                            bomb_set_place(game->bomb[(player_get_total_nb_bomb_set(player) + 1) % player_get_nb_bomb(player)], player);
                            map_set_cell_type(map,player_get_x(player),player_get_y(player),CELL_BOMB);
                            player_inc_total_nb_bomb_set(player);
                            player_inc_curr_nb_bomb_set(player);
                        }
                        break;
                    case SDLK_s: //Saves the game
                        save(game);
                        break;
                    case SDLK_p: //Pauses the game
                        pause(screen);
                        break;
                    default:
                        break;
                }

                break;
        }
    }
    return 0;
}

void game_display(t_game game, SDL_Surface* screen,SDL_Surface* grass) {
    int i, j;
   SDL_Rect grassplace;
    grassplace.x=0;
    grassplace.y=0;
    assert(game);
    t_map map = level_get_curr_map(game_get_curr_level(game));
    //                t_bomb bomb=game_get_bomb(game);    
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_BlitSurface(grass, NULL, screen, &grassplace);
    game_banner_display(game, screen);
    level_display(game_get_curr_level(game), screen);
    
       int nb = player_get_nb_bomb(game->player);
    for (i = 0; i <= nb; i++) { //bomb management
        bomb_display(game, game->player, game->bomb[i], screen, map);
        bomb_set_timer(game->bomb[i]);
    }
       
    if (get_life(game->player) == 0) { //check if the player's dead
        splashscreen(screen, 1);
        SDL_Quit();
    }
    if (game->start == 1) { //check if the user's is starting the game and places the monsters in consequence
        for (i = 0; i < map_get_width(map); i++) {
            for (j = 0; j < map_get_height(map); j++) {
                int type = map_get_cell_type(map, i, j);

                switch (type) { // type is encoded with 4 bits (the & 15 (1111)) is a mask to keep the four less significant bits)
                    case 6:
                        game->monster[game->nb_monster] = monster_init(i, j,1);
                        game_inc_nb_monster(game);
                        break;

                }
            }
        }
        game->start = 0;
    }
    if (game->nb_monster > 0) { //management of the monsters
        for (j = 0; j <= game->nb_monster - 1; j++) {
            if (monster_get_timer(game->monster[j]) == 10) {
                monster_set_current_way(game->monster[j],map);
                monster_move(game->monster[j], map);
                monster_reset_timer(game->monster[j]);
            } else monster_inc_timer(game->monster[j]);

            monster_display(game->monster[j], screen);

            game->autorisation_dec[j] = check_monster_player(game, game->autorisation_dec[j], j);
        }
    }
    player_display(game->player, screen);
    if (player_check_finish(game->player) == 1) { //if the player reaches the ultimate goal
        splashscreen(screen, 2);
        SDL_Quit();
    }


    SDL_Flip(screen); // SDL_Flip -- swaps screen buffers -- update

}

int game_update(t_game game, SDL_Surface* screen) {

    if (input_update(game, screen) == 1) {
        return 1; // exit game
    }

    return 0;
}

int check_monster_player(t_game game, int autorisation, int j) { //check if the player steps on a monster
    if (monster_get_x(game->monster[j]) == player_get_x(game->player) && monster_get_y(game->monster[j]) == player_get_y(game->player)) {
        if (autorisation == 1 && get_life(game->player) > 0) {
            player_dec_life(game->player);
            autorisation = 0;
        }


    } else autorisation = 1;
    return autorisation;

}

t_compose_type random_bonus(t_game game, int x, int y) { // returns a bonus based on probabilities
    int random1 = rand() % (101 - 1) + 1;
    int random2;
    int random3;
    if (random1 <= 80) { //proba bonus
        random2 = rand() % (101 - 1) + 1;
        if (random2 <= 40) { //proba bonus range
            random3 = rand() % (101 - 1) + 1;
            if (random3 <= 50) return CELL_CASE_RANGEINC;
            else return CELL_CASE_RANGEDEC;
        } else if (random2 > 40 && random2 <= 80) { //proba bonus nb
            random3 = rand() % (101 - 1) + 1;
            if (random3 <= 30) return CELL_CASE_BOMBINC;
            else return CELL_CASE_BOMBDEC;
        } else if (random2 > 80 && random2 <= 90) {
            return CELL_CASE_LIFE;
        }//proba bonus life
        else if (random2 > 90 && random2 <= 100) {
            game->monster[game->nb_monster] = monster_init(x, y,2);
            game->nb_monster++;
            return CELL_EMPTY;
        } //proba bonus monster

        //else{printf("le random1 vaut %d et random2vaut %d \n",random1,random2); return CELL_GOAL;}

    } else return CELL_EMPTY;
    return 0;
}

int splashscreen(SDL_Surface *screen, int num) { 
    int repeat = 1;
    while (repeat) {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
        SDL_Surface *splashscreen = NULL;
        SDL_Rect positionsplash;
        SDL_Surface * newgame[2] = {NULL};
        SDL_Rect positionnewgame;
        SDL_Surface * loadgame[2] = {NULL};
        SDL_Rect positionloadgame;
        SDL_Surface * mapeditor[2] = {NULL};
        SDL_Rect positionmapeditor;
        SDL_Surface * help[2] = {NULL};
        SDL_Rect positionhelp;


        SDL_Event event;
        int keep = 1;
        if (num == 0) {
            int highlightnew = 1;
            int highlightload = 0;
            int highlightmapeditor = 0;
            int highlighthelp= 0;
            positionsplash.x = 0;
            positionsplash.y = 0;
            positionnewgame.x = 90;
            positionnewgame.y = 230;
            positionloadgame.x = 90;
            positionloadgame.y = 300;
            positionmapeditor.x = 90;
            positionmapeditor.y = 370;
            positionhelp.x = 90;
            positionhelp.y = 440;
            splashscreen = IMG_Load(IMG_START);
            newgame[0] = IMG_Load(IMG_NEW_GAME);
            newgame[1] = IMG_Load(IMG_NEW_GAME_HIGH);
            loadgame[0] = IMG_Load(IMG_LOAD_GAME);
            loadgame[1] = IMG_Load(IMG_LOAD_GAME_HIGH);
            mapeditor[0] = IMG_Load(IMG_MAP_EDITOR);
            mapeditor[1] = IMG_Load(IMG_MAP_EDITOR_HIGH);
            help[0] = IMG_Load(IMG_HELP_BUTTON);
            help[1] = IMG_Load(IMG_HELP_BUTTON_HIGH);
            SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);
            SDL_BlitSurface(newgame[highlightnew], NULL, screen, &positionnewgame);
            SDL_BlitSurface(loadgame[highlightload], NULL, screen, &positionloadgame);
            SDL_BlitSurface(mapeditor[highlightmapeditor], NULL, screen, &positionmapeditor);
            SDL_BlitSurface(help[highlighthelp], NULL, screen, &positionhelp);
            SDL_Flip(screen);
            while (keep) {
                SDL_WaitEvent(&event);
                switch (event.type) {
                    case SDL_QUIT:
                        keep = 0;
                        repeat = 0;
                        return 4;
                        break;

                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {

                            case SDLK_RETURN: //manage your choice
                                keep = 0;

                                if (highlightnew == 1) {
                                    SDL_FreeSurface(splashscreen);
                                    repeat = 0;
                                    return 0;
                                } else if (highlightload == 1) {
                                    SDL_FreeSurface(splashscreen);
                                    repeat = 0;
                                    return 1;
                                } else if (highlightmapeditor == 1) {
                                    int test=map_editor(screen);
                                    if(test==2){return 4;}
                                    else keep = 0;
                                    
                                }
                                else if (highlighthelp == 1) {
                                     splashscreen = IMG_Load(IMG_HELP);
                                    SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);
                                    SDL_Flip(screen);
                                    int keep2=1;
                                    while (keep2) { //loop waits for the user to press a key to exit the splashscreen
                                                SDL_WaitEvent(&event);
                                        switch (event.type) {

                case SDL_QUIT:

                    SDL_FreeSurface(screen);
                    SDL_Quit();
                    break;

                case SDL_KEYDOWN:
                    SDL_FreeSurface(splashscreen);
                    keep2 = 0;
                    
                    break;
            }
        }
                                }


                                break;

                            case SDLK_DOWN: //moves the highlighting
                                if (highlightnew == 1) {
                                    highlightnew = 0;
                                    highlightload = 1;
                                    highlightmapeditor = 0;
                                    highlighthelp = 0;
                                } else if (highlightload == 1) {
                                    highlightnew = 0;
                                    highlightload = 0;
                                    highlightmapeditor = 1;
                                    highlighthelp = 0;
                                } else if (highlightmapeditor == 1) {
                                    highlightnew = 0;
                                    highlightload = 0;
                                    highlightmapeditor = 0;
                                    highlighthelp = 1;
                                }
                                else if (highlighthelp == 1) {
                                    highlightnew = 1;
                                    highlightload = 0;
                                    highlightmapeditor = 0;
                                    highlighthelp = 0;
                                }
                                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
                                SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);
                                SDL_BlitSurface(newgame[highlightnew], NULL, screen, &positionnewgame);
                                SDL_BlitSurface(loadgame[highlightload], NULL, screen, &positionloadgame);
                                SDL_BlitSurface(mapeditor[highlightmapeditor], NULL, screen, &positionmapeditor);
                                SDL_BlitSurface(help[highlighthelp], NULL, screen, &positionhelp);
                                SDL_Flip(screen);
                                break;
                            case SDLK_UP:
                                if (highlightnew == 1) {
                                    highlightnew = 0;
                                    highlightload = 0;
                                    highlightmapeditor = 0;
                                    highlighthelp = 1;
                                } else if (highlightload == 1) {
                                    highlightnew = 1;
                                    highlightload = 0;
                                    highlightmapeditor = 0;
                                    highlighthelp = 0;
                                } else if (highlightmapeditor == 1) {
                                    highlightnew = 0;
                                    highlightload = 1;
                                    highlightmapeditor = 0;
                                    highlighthelp = 0;
                                }
                                else if (highlighthelp == 1) {
                                    highlightnew = 0;
                                    highlightload = 0;
                                    highlightmapeditor = 1;
                                    highlighthelp = 0;
                                }
                                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
                                SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);
                                SDL_BlitSurface(newgame[highlightnew], NULL, screen, &positionnewgame);
                                SDL_BlitSurface(loadgame[highlightload], NULL, screen, &positionloadgame);
                                SDL_BlitSurface(mapeditor[highlightmapeditor], NULL, screen, &positionmapeditor);
                                SDL_BlitSurface(help[highlighthelp], NULL, screen, &positionhelp);
                                SDL_Flip(screen);
                                break;

                            default:
                                break;

                        }

                        break;
                }
            }
        } else if (num == 1) { //depending on the kind of slapshscreen you're asking for
            positionsplash.x = 0;
            positionsplash.y = 0;
            splashscreen = IMG_Load(IMG_LOSE);
            SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);

        } else if (num == 2) {
            positionsplash.x = 0;
            positionsplash.y = 0;
            splashscreen = IMG_Load(IMG_WIN);
            SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);
        } else if (num == 3) {
            positionsplash.x = 0;
            positionsplash.y = 0;
            splashscreen = IMG_Load(IMG_NEXT_LEVEL);
            SDL_BlitSurface(splashscreen, NULL, screen, &positionsplash);
        }

        SDL_Flip(screen);

        while (keep) { //loop waits for the user to press a key to exit the splashscreen
            SDL_WaitEvent(&event);
            switch (event.type) {

                case SDL_QUIT:
                    //keep = 0;
                    //repeat = 0;
                    SDL_FreeSurface(screen);
                    SDL_Quit();
                    break;

                case SDL_KEYDOWN:
                    SDL_FreeSurface(splashscreen);
                    keep = 0;
                    repeat = 0;
                    break;
            }
        }
    }
    return 2;
}

void save(t_game game) { 
    int i, j;

    t_map map = level_get_curr_map(game_get_curr_level(game));
    FILE* fichier = NULL;
    fichier = fopen("save.txt", "w+");

    //info player
    fprintf(fichier, "%d:%d:%d:%d:%d:%d\n", player_get_x(game->player), player_get_y(game->player), player_get_nb_bomb(game->player), player_get_nb_key(game->player), get_life(game->player), get_player_power(game->player));
    //info map
    fprintf(fichier, "%d:%d\n", level_get_num_level(game_get_curr_level(game)), level_get_num_map(game_get_curr_level(game)));
    fprintf(fichier, "%d:%d\n", map_get_width(map), map_get_height(map));
    for (i = 0; i <= map_get_height(map) - 1; i++) {
        for (j = 0; j <= map_get_width(map) - 1; j++) {
            if (map_get_cell_type(map, j, i) == CELL_SCENERY || map_get_cell_type(map, j, i) == CELL_BONUS) {
                fprintf(fichier, "%d%d ", map_get_cell_type(map, j, i), map_get_bonus_type(map, j, i));
            }
            else fprintf(fichier, "%d ", map_get_cell_type(map, j, i));
        }
        fprintf(fichier, "\n");

    }
    //info monsters
    for (i = 0; i <= game->nb_monster - 1; i++) {
        fprintf(fichier, "%d:%d\n", monster_get_x(game->monster[i]), monster_get_y(game->monster[i]));
    }
    fclose(fichier);


}

void pause(SDL_Surface* screen) {
    int keep = 1;
    SDL_Rect place;
        place.x=0;
        place.y=0;
        SDL_Surface* pause;
    SDL_Event event;

    while (keep) {
        
        
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
        pause=IMG_Load(IMG_PAUSE);
        SDL_BlitSurface(pause, NULL, screen, &place);
        SDL_Flip(screen);
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                keep = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_p:
                        keep = 0;
                    default:
                        break;

                }
        }
    }

}
