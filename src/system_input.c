#include "game_systems.h"

void OnInputEvent(event_t* ev, void* data){
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
      fn(in, ev->eid);
      break;
  }
}

void InputLoad(world_t* w, Entity e){
  input_comp_t* ic = GET_COMPONENT(w, e, input_comp_t, INPUT_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  notification n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, OnInputEvent, &p->pos, e.id );

  n = InputEvent_ToNotif(INPUT_EVENT_BINDING);
  for(int i = 0; i < ACT_DONE; i++){
    action_key_t akey = ic->input.actions[i];
    for(int j = 0; j < akey.num_keys; j++)
      TargetSubscribe(n, OnInputEvent, akey.fn, akey.keys[j]);
  }

}

void InputSystem(world_t* w, Entity e){
  input_comp_t* ic = GET_COMPONENT(w, e, input_comp_t, INPUT_ID);

  input_t* in = &ic->input;
  if(!InputCheck(in, in->turn))
    return;

}

