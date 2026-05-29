#include "system_events.h"
#include "process_event.h"

void PhysHandleEvent(world_t* w, Entity e, Entity tar , PhysicsEventID event){
  switch(event){
    //case PHYS_EVENT_COLL:
    case PHYS_EVENT_HIT:
      if(EntityHasRelation(w, e, "CollisionOf"))
        e = EntityGetRelationTarget(w, e, "CollisionOf");

      if(EntityHasRelation(w, tar, "CollisionOf"))
        return;

      GameEvent(PhysEvent_ToNotif(event), &tar, e.id);
      break;
  }
}
