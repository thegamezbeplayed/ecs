#include "game_register.h"
#include "game_systems.h"

void InputSystem(system_pool_t* s, component_registry_t* c){
  input_st *in = &s->input;
  input_c  *inputs = &c->input;
  sprite_c* spr = &c->sprites;
  
  for (int i = 0; i < inputs->map.size; i++){
    input_t* c = &inputs->dense[i];
    InputCheck(c, in->turn);

    if(cell_compare(c->step, CELL_UNSET))
      continue;

    Entity e = inputs->map.entities[i];
    if(!HasSprite(spr, e))
      continue;

    sprite_t* espr = SpriteGet(spr, e);
    espr->pos = Vector2Add(espr->pos,cell_to_vec(c->step, 0.1f));

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
