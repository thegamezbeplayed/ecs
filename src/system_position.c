#include "system_define.h"
#include "game_types.h"

void PositionSink(void* obs_data, void* sub, payload_t* pl ){
  position_t* p = obs_data;
  rigid_body_t* b = pl->data;

  Vector2 pos = b->bounds.pos;

  PositionSet(p, pos);
}

void PositionLoad(world_t* w, Entity e){

}

void PositionRegister(world_t* w){
  LookAddSink("Position", PositionSink);
}
