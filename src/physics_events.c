#include "system_events.h"
#include "process_event.h"

void PhysHandleEvent(world_t* w, Entity e, Entity tar , PhysicsEventID event){
  switch(event){
    //case PHYS_EVENT_COLL:
    case PHYS_EVENT_HIT:
      TraceLog(LOG_INFO, "PHYS EVENT %i hits %i", e.id, tar.id);
      if(EntityHasRelation(w, e, REL_ChildOf))
        e = EntityGetRelationTarget(w, e, REL_ChildOf);

      if(EntityHasRelation(w, tar, REL_ChildOf))
        return;

      GameEvent(PhysEvent_ToNotif(event), &tar, e.id);
      break;
  }
}
