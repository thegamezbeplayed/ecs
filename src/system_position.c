#include "system_define.h"
#include "game_types.h"

void PositionSink(void* obs_data, void* sub, void* ev_data){
  position_t* p = obs_data;
  rigid_body_t* b = ev_data;

  Vector2 pos = b->bounds.pos;

  PositionSet(p, pos);
}

void PositionLoad(world_t* w, Entity e){

}

void PositionRegister(world_t* w){
  LookAddSink("Position", PositionSink);
}
