#pragma once
#include <stdint.h>
#include "game_assets.h"

#define NUM_SYS 4
extern uint64_t ANIM_ID;
extern uint64_t NAME_ID;
extern uint64_t POS_ID;

typedef struct{
  anim_player_t* player;
}anim_component_t;

typedef struct{
  char  name[MAX_NAME_LEN];
}name_component_t;

typedef struct{
  position_t*   pos;
}pos_component_t;
