#include "system_define.h"
#include "game_types.h"

void OnPositionEvent(event_t* ev, void* data){
  position_t* p = data;
  rigid_body_t* b = ev->data;

  Vector2 pos = b->bounds.pos;

  PositionSet(p, pos);
}

void PositionLoad(world_t* w, Entity e){

}
