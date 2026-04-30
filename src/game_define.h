#pragma once
#include <stdint.h>
#include "game_assets.h"
#include "game_control.h"

#define NUM_SYS 4
extern uint64_t ANIM_ID;
extern uint64_t NAME_ID;
extern uint64_t POS_ID;
extern uint64_t INPUT_ID;

typedef struct{
  anim_player_t* player;
}anim_comp_t;

typedef struct{
  char  name[MAX_NAME_LEN];
}name_comp_t;

typedef struct{
  position_t*   pos;
}pos_comp_t;

typedef struct{
  input_t*   input;
}input_comp_t;
