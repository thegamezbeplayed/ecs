#include "system_define.h"
#include "tool_lookup.h"

void DebugSystem(world_t* w, Entity e){
  debug_t* d = ComponentGet(w, e, DEBUG_ID);

  Entity rel = EntityGetRelationTarget(w, e, REL_Debugs);

  position_t* p = ComponentGet(w, rel, POS_ID);

  state_t* s = ComponentGet(w, rel, STATE_ID);

  strcpy(d->str, STATE_LOOKUP[s->state].str);

  DrawText(d->str, p->pos.x, p->pos.y-12, 4, BLUE);
}
