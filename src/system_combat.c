#include "system_define.h"

void CombatPrep(world_t* w){
  component_pool_t* teams = w->pools[TEAM_ID];

  if(!teams->dirty)
    return;

  system_t* c = HashGet(&w->sys_map, hash_str_64("Combat"));
  entity_iter_t* iter = SystemGetIter("Combat");
  EntityIterStart(w, iter, c);

  teams->dirty = false;

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
