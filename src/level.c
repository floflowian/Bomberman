#include "../include/game.h"
#include "../include/level.h"
#include "../include/map.h"

struct s_level {
    t_map *maps; // array of the level's maps
    ushort nb_maps; // nb maps of the level
    ushort cur_map; // the current map
    int num_level;
};

t_level level_get_level(int num, t_map map, int num_level, int num_map) {
    t_level level = malloc(sizeof (struct s_level));
    switch (num) {
        case 0: //if the user requires a new game
            level->nb_maps = 2;
            level->cur_map = 0;
            level->maps = malloc(sizeof (*level->maps));
            level->num_level = 1;
            level->maps[0] = map_get_default(level);
             

            return level;
            break;
        case 1: //if the user requires a saved game
            
            level->nb_maps = 2;
            level->cur_map = num_map;
            level->maps = malloc(sizeof (*level->maps));
            level->num_level = num_level;
            level->maps[level->cur_map] = map;
            return level;
            break;
    }
    return NULL;
}

int level_get_num_map(t_level level) {

    return level->cur_map;
}
void level_set_map(t_level level,int num, t_map map){
    level->maps[num]=map;
}
int level_get_num_level(t_level level) {

    return level->num_level;
}

t_map level_get_curr_map(t_level level) {
    return level->maps[level->cur_map];
}

t_map level_get_map(t_level level, int num) {
    assert(num <= level->nb_maps);
    return level->maps[num];
}

void level_map_inc(t_level level) {
    level->cur_map++;
    level->maps[level->cur_map] = map_get_default(level);

}

void level_reset_cur_map(t_level level) {
    level->cur_map = 0;
    level->maps[level->cur_map] =map_get_default(level);
}

void level_inc(t_level level) {
    level->num_level++;
    level->maps[level->cur_map] = map_get_default(level);


}

void level_free(t_level level) {
    for (int i = 0; i <= level->cur_map; i++) {
        map_free(level->maps[i]);
    }
    free(level->maps);
}

void level_display(t_level level, SDL_Surface* screen) {
    map_display(level->maps[level->cur_map], screen);
}
