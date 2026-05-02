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
      dir = CELL_LEFT;
      gi->angle = 180;
      break;
    case KEY_D:
    case KEY_L:
    case KEY_RIGHT:
      dir = CELL_RIGHT;
      gi->angle = 0;
      break;
    case KEY_W:
    case KEY_J:
    case KEY_UP:
      dir = CELL_UP;
      gi->angle = 90;
      break;
    case KEY_S:
    case KEY_K:
    case KEY_DOWN:
      gi->angle = 270;
      dir = CELL_DOWN;
      break;
    default:
      return BEHAVIOR_FAILURE;
      break;
  }

  gi->step = dir;

  gi->last_act = ACT_MOVE;
  GameEvent(InputEvent_ToNotif(INPUT_EVENT_MOVE), gi, PLAYER);
  return BEHAVIOR_SUCCESS;
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

  int key = GetKeyPressed();

  notification n = 0;
  if(key > 0){
    n = InputEvent_ToNotif(INPUT_EVENT_BINDING);
    GameEvent(n, gi, key);
    gi->last_key = key;
  }
  else if(IsKeyDown(gi->last_key)){
    n = InputEvent_ToNotif(INPUT_EVENT_BINDING);
    GameEvent(n, gi, gi->last_key);
  }
  else if(IsKeyReleased(gi->last_key)){
    n = InputEvent_ToNotif(INPUT_EVENT_KEY_RELEASE);
    GameEvent(n, gi, gi->last_act);
  }

}
