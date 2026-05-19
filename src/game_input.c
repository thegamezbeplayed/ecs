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

  SubjectStore(name, NULL);

}

action_key_t* InputGetAction(KeyboardKey key){
  hash_key_t hash = hash_64_from_int(key);
  return HashGet(&MACRO_KEYS, hash);
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

  if(key > 0){
    gi->last_key = key;
    return true;
  }
  else if(IsKeyReleased(gi->last_key)){
    gi->last_key = 0;
    return true;
  }

  return false;
}
