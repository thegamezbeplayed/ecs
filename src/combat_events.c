#include "system_events.h"

void CombatHandleEvent(notification n, Entity *tar, Entity agg){
  switch(EVENT_ID(n)){
    case PHYS_EVENT_HIT:
      name_t* aname = ComponentGet(&world, agg, NAME_ID);
      name_t* tname = ComponentGet(&world, *tar, NAME_ID);

      stat_t* health = ComponentGet(&world, *tar, STAT_ID);
      TraceLog(LOG_INFO, "=== COMBAT HANDLE ===\n %s hits %s", aname, tname);
      break;
  }
}
