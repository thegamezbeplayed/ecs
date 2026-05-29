#include "system_define.h"
#include "tool_lookup.h"
#include "debug_define.h"


void DebugGetCompData(world_t* w, Entity e, comp_id_t cid, char* buf){
  char tmp[MAX_NAME_LEN];
 
  if(cid == POS_ID){
    position_t* p = ComponentGet(w, e, POS_ID);

    sprintf(tmp, "Grid %i, %i", p->grid_bucket.x, p->grid_bucket.y); 
  }
  else if(cid == STAT_ID){
    stat_t* s = ComponentGet(w, e, STAT_ID);
    sprintf(tmp, "%s: %i / %i", s->name, s->current, s->max);
  }
  else
    return;

  strcat(buf, tmp);
}

void DebugSystem(world_t* w, Entity e){
  debug_t* d = ComponentGet(w, e, DEBUG_ID);

  if(!EntityHasRelation(w, e, "DebuggerOf"))
    return;

  Entity rel = EntityGetRelationTarget(w, e, "DebuggerOf");

  d->str[0] = '\0';
  position_t* p = ComponentGet(w, rel, POS_ID);
  for(int i = 0; i < d->num_comps; i++){
    DebugGetCompData(w, rel, d->comps[i], d->str);
    strcat(d->str,"\n");
  }

  Vector2 offset = VEC_NEW(16, 24);
  Vector2 dpos = VEC_SUB(p->pos, offset);
  DrawDebugText(d->str, 6, dpos, BLACK, BLUE);
}

void DebugUse(world_t* w){
  InitDebug();
}
