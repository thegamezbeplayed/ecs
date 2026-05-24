#include "process_event.h"
#include "game_control.h"
#include "component_define.h"

static macro_map_t MACRO_KEYS;

void InitMacroKeys(int cap){
  HashInit(&MACRO_KEYS, next_pow2_int(cap));
}

void RegisterMacro(action_key_t *a) {
  hash_key_t hash = hash_64_from_int(a->key);
  HashPut(&MACRO_KEYS, hash, a);
  SubjectRegister(a->name);
}

action_key_t* InputGetAction(KeyboardKey key){
  hash_key_t hash = hash_64_from_int(key);
  return HashGet(&MACRO_KEYS, hash);
}

bool InputInit(void* comp, component_entry_t* j){
  input_t* in = comp;

  return ParseInputComponent(j->data, in);
}

input_t* InitInput(void){
  input_t* in = GameCalloc("InitInput", 1, sizeof(input_t));

  in->turn = -1;
}

bool InputSetKey(input_t* gi, KeyboardKey k){
  action_key_t* ak = InputGetAction(k);

  if(ak)
    gi->last_key = k;
  else
    gi->last_key = KEY_NULL;

  return gi->last_key > KEY_NULL;
}

bool InputCheck(input_t* gi, Entity e){
  if(IsKeyDown(KEY_SPACE))
    DO_NOTHING();

  int key = GetKeyPressed();

  if(key > 0)
    return InputSetKey(gi, key);
  else if(IsKeyReleased(gi->last_key))
    return InputSetKey(gi, key);
  else if(IsKeyDown(gi->last_key))
    return InputSetKey(gi, gi->last_key);
 
  return false;
}
