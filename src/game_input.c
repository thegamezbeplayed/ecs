#include "game_types.h"
#include "game_process.h"
#include "game_control.h"

input_t player_input;

BehaviorStatus InputActionMove(input_t* gi, KeyboardKey k){
  Cell dir = CELL_UNSET;

  switch(k){
    case KEY_A:
    case KEY_H:
    case KEY_LEFT:
      strcpy(gi->move , "MOVE_left");
      dir = CELL_LEFT;
      break;
    case KEY_D:
    case KEY_L:
    case KEY_RIGHT:
      strcpy(gi->move , "MOVE_right");
      dir = CELL_RIGHT;
      break;
    case KEY_W:
    case KEY_J:
    case KEY_UP:
      strcpy(gi->move , "MOVE_up");
      dir = CELL_UP;
      break;
    case KEY_S:
    case KEY_K:
    case KEY_DOWN:
      strcpy(gi->move , "MOVE_down");
      dir = CELL_DOWN;
      break;
    default:
      break;
  }

  gi->step = dir;
}


input_t* InitInput(void){
  input_t* in = GameCalloc("InitInput", 1, sizeof(input_t));

  in->turn = -1;
  in->step =  CELL_UNSET;
  in->actions[ACT_MOVE] = (action_key_t){
    ACT_MOVE,12,{KEY_H, KEY_J, KEY_K, KEY_L, KEY_D,KEY_A,KEY_W,KEY_S,KEY_LEFT, KEY_RIGHT,KEY_UP,KEY_DOWN},InputActionMove, 0};
}

bool InputCheck(input_t* gi, int turn){
  if(IsKeyDown(KEY_SPACE))
      DO_NOTHING();
  
  if(turn == gi->turn)
    return false;

  gi->step = CELL_UNSET;
  for(int i = 0; i < ACT_DONE; i++){
    action_key_t akey = gi->actions[i];

    for (int j = 0; j< akey.num_keys; j++){
      KeyboardKey k = akey.keys[j];

      if(!IsKeyDown(k))
        continue;

      akey.fn(gi, k);
    }
  }
}
