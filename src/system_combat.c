#include "system_define.h"
#include "system_events.h"

void CombatSink(void* obs_data, void* sub, payload_t* pl){
  team_t* t = obs_data;
  switch(EVENT_ID(pl->event)){
    case TEAM_EVENT_CHARM:
      Entity charmed = EntityGet(&world.manager, pl->type_id);
      CombatHandleBehavior(charmed);

      break;
      
  }
}

void CombatOnEvent( event_t *event, void* user_data){
  Entity e = EntityGet(&world.manager, event->eid);
  Entity *other = event->data;

  CombatHandleEvent(event->type, other, e);
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
  LookAddHandler("Combat", CombatOnEvent);
}

void TeamSystem(world_t* w, Entity e){
  team_t* t = ComponentGet(w, e, TEAM_ID);

  if(t->event == TEAM_EVENT_NONE)
    return;

  position_t* p =  ComponentGet(w, e, POS_ID);

  if(!p)
    return;

  neighbor_list_t list = SpatialHashGridGetNeighbors(&w->grid, p->pos, 128, e.id);

  for (int i = 0; i < list.count; i++) {
    uint32_t id = list.items[i].id;
    team_t* other = ComponentGetByID(w, id, TEAM_ID);
    if (!other) continue;

    if (t->target_id != other->id)
      continue;

    Vector2 epos = list.items[i].pos;

    if (VEC_DIST(epos, p->pos) > 64 )
      continue;

    other->id = t->id;

    Entity target = EntityGet(&w->manager, id);
    ComponentUpdate(w, target, TEAM_ID, TeamEvent_ToNotif(t->event));
  }

}

void TeamRegister(world_t* w, Entity e){
  team_t* t = GET_COMPONENT(w, e , team_t, TEAM_ID);

  TeamRegisterMember(t->name, e);
}

void TeamLoad(world_t* w){
  TeamMapInit(8);
}
