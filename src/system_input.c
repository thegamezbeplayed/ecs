#include "game_systems.h"

void OnInputEvent(event_t* ev, void* data){
  input_t* in   = ev->data;
  position_t* p = data;

  notification n = PhysEvent_ToNotif(PHYS_EVENT_ACCEL);

  p->dir_step = cell_to_vec(in->step,1);
  GameEvent(n, p, ev->eid); 

  //PositionAddStep(p, cell_to_vec(in->step, 0.05f));
}

void InputLoad(world_t* w, Entity e){
  input_comp_t* ic = GET_COMPONENT(w, e, input_comp_t, INPUT_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  notification n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, OnInputEvent, p->pos, e.id );
}

void InputSystem(world_t* w, Entity e){
  input_comp_t* ic = GET_COMPONENT(w, e, input_comp_t, INPUT_ID);

  input_t* in = ic->input;
  if(!InputCheck(in, in->turn))
    return;

}

