#include "process_event.h"
#include "game_control.h"
#include "component_define.h"

static macro_map_t MACRO_KEYS;

void InitMacroKeys(int cap){
  HashInit(&MACRO_KEYS, next_pow2_int(cap));
}

void RegisterMacro(char* name, KeyboardKey key){
  hash_key_t hash = hash_64_from_int(key);

  action_key_t *a = GameCalloc("RegisterMacro", 1, sizeof(action_key_t));

  a->key = key;
  strcpy(a->name, name);
  HashPut(&MACRO_KEYS, hash, a);
}

bool InputInit(void* comp, component_entry_t* j){
  input_t* in = comp;

  in->turn = -1;
  in->step =  CELL_UNSET;
  return true;
}

input_t* InitInput(void){
  input_t* in = GameCalloc("InitInput", 1, sizeof(input_t));

  in->turn = -1;
  in->step =  CELL_UNSET;
}

bool InputCheck(input_t* gi, Entity e){
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
    GameEvent(n, gi, e.id);
    gi->last_key = 0;
    return false;
  }
  else
    return false;

  notification an;
  switch(gi->last_act){
    case ACT_MOVE:
      an = InputEvent_ToNotif(INPUT_EVENT_MOVE);
      break;
    case ACT_ATTACK:
      an = InputEvent_ToNotif(INPUT_EVENT_ATTACK);
      break;
    default:
      return false;
      break;
  }
  
  GameEvent(an, gi, e.id);

  return true;
}
