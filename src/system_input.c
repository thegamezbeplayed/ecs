#include "process_event.h"
#include "system_define.h"
#include "process_event.h"
#include "tool_lookup.h"

void InputLoad(world_t* w, Entity e){
  input_t* in = GET_COMPONENT(w, e, input_t, INPUT_ID);

  for(int i = 0; i < MAX_MACROS; i++){
    action_key_t* a = &in->action_keys[i];
    if(a->type == ACT_MOVE)
      a->dir = StringToVector2(a->name);
    
    RegisterMacro(a);
  }
}

void InputSystem(world_t* w, Entity e){
  input_t* in = GET_COMPONENT(w, e, input_t, INPUT_ID);

  if(!InputCheck(in, e)){
    ComponentClearUpdate(w, e, INPUT_ID);
    return;
  }

  KeyboardKey k = in->last_key;
  action_key_t* ak = InputGetAction(k);
  if(!ak || ak->type == ACT_NONE){ 
    ComponentClearUpdate(w, e, INPUT_ID);
    return;
  }

  ComponentUpdate(w, e, INPUT_ID);
  SubjectNotify(ak->name, in);
}

void InputRegister(world_t* w){
  InitMacroKeys(MAX_MACROS);//TODO USE A DEFINE
}

