#include "system_define.h"
#include "process_event.h"
#include "tool_lookup.h"

void SubscriptionSystem(world_t* w, Entity e){
  subscription_t* s = ComponentGet(w, e, SUBSCRIBE_ID);

  EventCallback handle = LookupSystemHandler(s->listener);
  if(!handle){
    TraceLog(LOG_WARNING, "=== SUBSCRIPTION SYSTEM ===\n Unable to find System Event Handler for %s", s->listener); 
    return;
  }
  Entity tar = e;
  switch(s->type){
    case OBJ_ENT:

      break;
    case OBJ_REL:
      if(!EntityHasRelation(w, e, REL_EventOf))
        return;

      tar = EntityGetRelationTarget(w, e, REL_EventOf);
      break;
  }


  SubscribeEntity(s->event, handle, &tar, tar.id);
}
