#include "game_systems.h"
#include "game_types.h"

void OnPositionEvent(event_t* ev, void* data){
  position_t* p = data;
  rigid_body_t* b = ev->data;

  Vector2 step = b->vel;

  PositionAddStep(p, step);

  TraceLog(LOG_INFO, "==== POSITION EVENT === \n Ent: %i new [%0.2f, %0.2f", ev->eid, p->vpos.x, p->vpos.y);
}

void PositionLoad(world_t* w, Entity e){

}
