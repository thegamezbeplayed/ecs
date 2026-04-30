#include "game_register.h"
#include "game_enum.h"

void OnSystemEvent(event_t* ev, void* data){
  system_t* s = data;
  world_t*  w = ev->data;
  switch((GameEventID)EVENT_ID(ev->type)){
    case GAME_EVENT_STEP:
      UpdateType u = ev->eid;
      SystemTick(w, s, u);
      break;
    case GAME_EVENT_STATE:
      GameState state = ev->eid;
      SystemSet(w, s, state);
      break;
  }

}

system_t* SystemRegister(world_t* w, SystemCB tick[UPDATE_DONE], SystemCB set[GAME_DONE]){
  system_t* s = &w->systems[w->num_sys];
  memset(s, 0, sizeof(system_t));
  s->index = w->num_sys++;
  for (int i = 0; i < UPDATE_DONE; i++){
    if(!tick[i])
      continue;

    s->tick[i] = tick[i];
    uint64_t n = GameEvent_ToNotif(GAME_EVENT_STEP);
    TargetSubscribe(n, OnSystemEvent, s, i);
  }
  for (int i = 0; i < GAME_DONE; i++){
    if(!set[i])
      continue;

    s->set[i] = set[i];
    uint64_t n = GameEvent_ToNotif(GAME_EVENT_STATE);
    TargetSubscribe(n, OnSystemEvent, s, i);

  }

  return s;
}

component_pool_t* ComponentQueryInner(world_t* w, system_t* s) {
  component_pool_t* best = NULL;

  for (int i = 0; i < s->term_count; i++) {
    comp_id_t id = s->terms[i];
    component_pool_t* p = w->pools[id];

    if (!p) return NULL;
    if (p->size == 0) return NULL;

    if (!best || p->size < best->size) {
      best = p;
    }
  }

  return best;
}


void SystemTick(world_t* w, system_t* s, UpdateType u){
  if(!s->tick[u])
    return;

  component_pool_t* base = ComponentQueryInner(w, s);
  if (!base) return;

  for (int i = 0; i < base->size; i++) {
    Entity e = { base->entities[i], w->manager.generation[base->entities[i]] };

    bool match = true;

    for (int t = 0; t < s->term_count; t++) {
      component_pool_t* pool = w->pools[s->terms[t]];

      if (pool->sparse[e.id] == -1) {
        match = false;
        break;
      }
    }

    if (!match) continue;

    s->tick[u](w, e);
  }
}

void SystemSet(world_t* w, system_t* s, GameState g){
  if(!s->set[g])
    return;

  component_pool_t* base = ComponentQueryInner(w, s);
  if (!base) return;

  for (int i = 0; i < base->size; i++) {
    Entity e = { base->entities[i], w->manager.generation[base->entities[i]] };

    bool match = true;

    for (int t = 0; t < s->term_count; t++) {
      component_pool_t* pool = w->pools[s->terms[t]];

      if (pool->sparse[e.id] == -1) {
        match = false;
        break;
      }
    }

    if (!match) continue;

    s->set[g](w, e);
  }

}
