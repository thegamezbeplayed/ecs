#include "system_define.h"
#include "system_events.h"

void CombatSink(void* obs_data, void* sub, payload_t* pl){
  switch(EVENT_ID(pl->event)){
    case PHYS_EVENT_HIT:
      Entity *tar = obs_data;;
      Entity *agg = pl->data;
      CombatHandlePhys(tar, agg);
      break;

  }
}

void CombatPrep(world_t* w){
  component_pool_t* teams = w->pools[TEAM_ID];

  if(!teams->dirty)
    return;

  system_t* c = HashGet(&w->sys_map, hash_str_64("Combat"));
  entity_iter_t* iter = SystemGetIter("Combat");
  EntityIterStart(w, iter, c);

  teams->dirty = false;

}

void CombatRegister(world_t* w){
  LookAddSink("Stat", CombatSink);

}

void TeamSystem(world_t* w, Entity e){

}

void TeamRegister(world_t* w, Entity e){
  team_t* t = GET_COMPONENT(w, e , team_t, TEAM_ID);

  TeamRegisterMember(t->name, e);
}

void TeamLoad(world_t* w){
  TeamMapInit(8);
}
