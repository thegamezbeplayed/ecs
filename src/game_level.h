#ifndef __GAME_LVL__
#define __GAME_LVL__

#include "game_common.h"

typedef enum{
  LVL_GRID,
  LVL_NONE
}LevelType;

typedef struct{
  char        name[MAX_NAME_LEN];
  LevelType   type;
  int         wid, hei;
}level_t;

level_t* InitLevel(char* name, int wid, int hei);
#endif
