#include "game_register.h"
#include "game_systems.h"

void OnInputEvent(event_t* ev, void* data){
  rigid_body_t* b = data;
  input_t*      in = ev->data;

  rigid_body_t* test = &world.bodies.dense[ev->eid];
  Vector2 dir = cell_to_vec(in->step, 1);
  RigidBodySteer(b, dir);
}

void InputSystem(system_pool_t* s, component_registry_t* c){
  input_st *in = &s->input;
  input_c  *inputs = &c->input;
  position_c* pos = &c->pos;

  for (int i = 0; i < inputs->map.size; i++){
    input_t* c = &inputs->dense[i];
    if(!InputCheck(c, in->turn))
      continue;

    /*
    position_t* p = &pos->dense[i];


    Entity e = pos->map.entities[i];
    GameEvent(c->move, p, e.id);
    //PositionAddStep(p, cell_to_vec(c->step, 0.1));
*/
    in->step = true;
  }
}

void InputTurn(system_pool_t* s, component_registry_t* c){
  input_st *in = &s->input;

  if(!in->step)
    return;

  in->step = false;
  in->turn++;
}

void OnPositionEvent(event_t* ev, void* data){
  position_t* p = data;
  rigid_body_t* b = ev->data;

  Vector2 step = b->vel;

  PositionAddStep(p, step);
}

void InitInputSystem(input_st*){
  RegisterScheduleStep(UPDATE_FRAME, InputSystem);
  RegisterScheduleStep(UPDATE_FIXED, InputTurn);
}

void LoadPositions(system_pool_t* s, component_registry_t* c){

}

void InitPositionSystem(position_st*){
  RegisterScheduleState(GAME_READY, LoadPositions);

}
