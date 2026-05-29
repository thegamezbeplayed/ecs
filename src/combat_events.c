#include "system_events.h"

void CombatHandleEvent(notification n, Entity *tar, Entity agg){
  switch(EVENT_ID(n)){
    case PHYS_EVENT_HIT:
      name_t* aname = ComponentGet(&world, agg, NAME_ID);
      name_t* tname = ComponentGet(&world, *tar, NAME_ID);

      stat_t* health = ComponentGet(&world, *tar, STAT_ID);
      if(!EntityHasRelation(&world, agg, "OwnsDamageOf")){
        TraceLog(LOG_WARNING, "=== COMBAT EVENT ===\n No damage stat for Ent: %s", aname);
        return;
      }
      Entity d_rel  = EntityGetRelationTarget(&world, agg, "OwnsDamageOf");
      stat_t* dmg = ComponentGet(&world, d_rel, STAT_ID);
      
      StatChangeValue(health, StatRollRange(dmg));

      break;
  }
}
