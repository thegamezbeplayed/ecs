#include "system_events.h"

void CombatHandlePhys(Entity *tar, Entity *agg){

  TraceLog(LOG_INFO, "=== COMBAT HANDLE ===\n Entity %i hits Entity %i", tar->id, agg->id);
}
