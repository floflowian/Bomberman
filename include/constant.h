/*
 * constant.h
 *
 *  Created on: 15 nov. 2010
 *      Author: Nicolas Aucouturier, Laurent R�veill�re
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bomberman 2013"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40
#define WINDOW_BPP      16

// Size (# of pixels) of banner
#define SIZE_LINE		4
#define SIZE_BANNER		40


// Time management
#define DEFAULT_GAME_FPS 30

// Sprites general
#define IMG_MAP_CASE                             "sprite/wood_box.png"
#define IMG_MAP_GOAL                            "sprite/triforce.png"
#define IMG_MAP_KEY                               "sprite/key.png"
#define IMG_MAP_DOOR		"sprite/door.png"
#define IMG_MAP_CLOSED_DOOR	"sprite/closed_door.png"
#define IMG_LOSE                                     "sprite/lose.jpg"
#define IMG_NEXT_LEVEL                           "sprite/next_level.jpg"
#define IMG_WIN                                        "sprite/win.jpg"
#define IMG_BOMBERWOMAN                     "sprite/zelda.png"
#define IMG_START                                    "sprite/start.jpg"
#define IMG_PAUSE                                    "sprite/pause.jpg"
// Scenery elements
#define IMG_MAP_STONE		"sprite/stone.png"
#define IMG_MAP_TREE                            "sprite/tree.png"
#define IMG_CURSOR                              "sprite/cursor.png"
#define IMG_GRASS                              "sprite/grass.jpg"
// Sprites of Banner
#define IMG_BANNER_LINE		"sprite/banner_line.png"
#define IMG_BANNER_LIFE		"sprite/banner_life.png"
#define IMG_BANNER_BOMB		"sprite/bomb3.png"
#define IMG_BANNER_RANGE                    "sprite/banner_range.png"
#define IMG_BANNER_KEY                         "sprite/key.png"
#define IMG_BANNER_LEVEL                      "sprite/triforce.png"
#define IMG_BANNER_0		"sprite/banner_0.jpg"
#define IMG_BANNER_1		"sprite/banner_1.jpg"
#define IMG_BANNER_2		"sprite/banner_2.jpg"
#define IMG_BANNER_3		"sprite/banner_3.jpg"
#define IMG_BANNER_4		"sprite/banner_4.jpg"
#define IMG_BANNER_5		"sprite/banner_5.jpg"
#define IMG_BANNER_6		"sprite/banner_6.jpg"
#define IMG_BANNER_7		"sprite/banner_7.jpg"
#define IMG_BANNER_8		"sprite/banner_8.jpg"
#define IMG_BANNER_9		"sprite/banner_9.jpg"
#define IMG_NEW_GAME                            "sprite/newgame.png"
#define IMG_NEW_GAME_HIGH                   "sprite/newgamehigh.png"
#define IMG_LOAD_GAME                            "sprite/loadgame.png"
#define IMG_LOAD_GAME_HIGH                   "sprite/loadgamehigh.png"
#define IMG_MAP_EDITOR                            "sprite/mapeditor.png"
#define IMG_MAP_EDITOR_HIGH                   "sprite/mapeditorhigh.png"
#define IMG_HELP                                        "sprite/help.png"
#define IMG_HELP_BUTTON                            "sprite/helpbutton.png"
#define IMG_HELP_BUTTON_HIGH                   "sprite/helpbuttonhigh.png"
#define IMG_ERASER                                     "sprite/eraser.png"
// Sprites of Bombs
#define IMG_BOMB_TTL1       "sprite/bomb1.png"
#define IMG_BOMB_TTL2       "sprite/bomb2.png"
#define IMG_BOMB_TTL3       "sprite/bomb3.png"
#define IMG_BOMB_TTL4       "sprite/bomb4.png"
#define IMG_BOMB_EXP		"sprite/explosion.png"
// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_LIFE			  "sprite/banner_life.png"


// Sprites of Players
#define IMG_PLAYER_LEFT     "sprite/player_left.png"
#define IMG_PLAYER_UP       "sprite/player_up.png"
#define IMG_PLAYER_RIGHT    "sprite/player_right.png"
#define IMG_PLAYER_DOWN     "sprite/player_down.png"

// Sprites of Monsters
#define IMG_MONSTER_LEFT     "sprite/monster_left.png"
#define IMG_MONSTER_UP       "sprite/monster_up.png"
#define IMG_MONSTER_RIGHT    "sprite/monster_right.png"
#define IMG_MONSTER_DOWN     "sprite/monster_down.png"
#define IMG_MONSTER2_LEFT     "sprite/monster2_left.png"
#define IMG_MONSTER2_UP       "sprite/monster2_up.png"
#define IMG_MONSTER2_RIGHT    "sprite/monster2_right.png"
#define IMG_MONSTER2_DOWN     "sprite/monster2_down.png"


// Maximum power of a bomb
#define BOMB_RANGE_MAX      4
// Default power of a bomb
#define BOMB_RANGE_DEFAULT	1

// Lifetime of a bomb in second
#define BOMB_LIFETIME		4

// Number of Monster move per second
#define MONSTER_MOVE_PER_SEC 3


// Bonus
// Probability that a box contains a bonus
#define PROBA_BONUS 80

#define PROBA_BONUS_BOMB_RANGE 40

// Increase the power of a bomb +1
#define PROBA_BONUS_BOMB_RANGE_INC 50

// Decrease the power of a bomb -1
#define PROBA_BONUS_BOMB_RANGE_DEC 50

#define PROBA_BONUS_BOMB_NB    40

// Increase the number of bombs
#define PROBA_BONUS_BOMB_NB_INC  30
// Decrease the number of bombs
#define PROBA_BONUS_BOMB_NB_DEC  70

#define PROBA_BONUS_MONSTER      10
#define PROBA_BONUS_LIFE      10

#define MAP_1 "map/map_c_1.txt"
#define MAP_INC 10
#define LEVEL_INC 20
#define FINISH 30
#endif /* CONSTANT */
