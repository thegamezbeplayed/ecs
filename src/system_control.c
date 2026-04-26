#include "game_register.h"
#include "game_systems.h"

void InputSystem(system_pool_t* s, component_registry_t* c){
  input_st *in = &s->input;
  input_c  *inputs = &c->input;
  position_c* pos = &c->pos;

  for (int i = 0; i < inputs->map.size; i++){
    input_t* c = &inputs->dense[i];
    InputCheck(c, in->turn);

    position_t* p = &pos->dense[i];

    if(cell_compare(c->step, CELL_UNSET))
      continue;

    Entity e = pos->map.entities[i];
    GameEvent(c->move, p, e.id);
    PositionAddStep(p, cell_to_vec(c->step, 0.1));

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

void InitInputSystem(input_st*){
  RegisterScheduleStep(UPDATE_FRAME, InputSystem);
  RegisterScheduleStep(UPDATE_FIXED, InputTurn);
}

void InitPositionSystem(position_st*){

}
