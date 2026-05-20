#include "process_event.h"
#include "system_define.h"
#include "game_control.h"
#include "process_event.h"
#include "tool_lookup.h"

void OnInputEvent(event_t* ev, void* data){
/*
  input_t* in   = ev->data;
  switch(EVENT_ID(ev->type)){
    case INPUT_EVENT_MOVE:
      position_t* p = data;
      notification n = PhysEvent_ToNotif(PHYS_EVENT_ACCEL);
      p->dir_step = cell_to_vec(in->step,1);
      GameEvent(n, p, ev->eid); 
      break;
    case INPUT_EVENT_BINDING:
      ActionKeyCallback fn = data;

      if(fn(in, ev->eid) == BEHAVIOR_SUCCESS)
        GameEvent(InputEvent_ToNotif(INPUT_EVENT_MOVE), in, ev->eid);

      break;
  }
  */
}

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

  if(!InputCheck(in, e))
    return;

  KeyboardKey k = in->last_key;
  action_key_t* ak = InputGetAction(k);

  if(!ak || ak->type == ACT_NONE)
    return;

  SubjectNotify(ak->name, in);

  ComponentUpdate(w, e, INPUT_ID);
}

void InputRegister(world_t* w){
  InitMacroKeys(MAX_MACROS);//TODO USE A DEFINE
}

