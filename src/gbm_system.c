#include "game_define.h"
#include "game_enum.h"
#include "process_event.h"

hash_map_t SYS_ITERS;

void SystemIterInit(int cap){
  HashInit(&SYS_ITERS, next_pow2_int(cap));
}

uint64_t SystemRegisterIter(const char* str){
  hash_key_t key = hash_str_64(str);
  entity_iter_t* e = SystemGetIter(str);
  if(!e){
    e = GameCalloc("SystemRegisterIter", 1, sizeof(entity_iter_t));
    HashPut(&SYS_ITERS, key, e);
  }

  return key;
}

entity_iter_t* SystemGetIter(const char* str){
  hash_key_t key = hash_str_64(str);
  return HashGet(&SYS_ITERS, key);
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

void SystemState(world_t* w, system_t* s, GameState g){
  if(!s->state[g])
    return;

  s->state[g](w);
}
 
void SystemStep(world_t* w, system_t* s, UpdateType u){
  if(!s->step[u])
    return;

  s->step[u](w);
}
 
void SystemTick(world_t* w, system_t* s, UpdateType u){
  if(!s->tick[u])
    return;

  component_pool_t* base = ComponentQueryInner(w, s);
  if (!base) return;

  /*
  if(s->fn_iter[u])
    *w->iter = EntityIterStart(w, s);
*/
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
    if(!EntityReady(&w->manager, e))
      continue;

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

    if(!EntityReady(&w->manager, e))
      continue;

    s->set[g](w, e);
  }

}

void OnSystemEvent(event_t* ev, void* data){
  system_t* s = data;
  world_t*  w = ev->data;
  UpdateType u;
  GameState state;
  switch((GameEventID)EVENT_ID(ev->type)){
    case GAME_EVENT_SYNC:
      u = ev->eid;
      SystemTick(w, s, u);
      break;
    case GAME_EVENT_STEP:
      u = ev->eid;
      SystemStep(w, s, u);
      break;
    case GAME_EVENT_SET:
      state = ev->eid;
      SystemSet(w, s, state);
      break;
    case GAME_EVENT_STATE:
      state = ev->eid;
      SystemState(w, s, state);
      break;
  }
}

system_t* SystemRegister(world_t* w, SystemCB tick[UPDATE_DONE], SystemCB set[GAME_DONE], SystemFn init){
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

system_t* SystemCreate(world_t* w, system_define_t* def){
  system_t* s = &w->systems[w->num_sys];
  memset(s, 0, sizeof(system_t));
  s->index = w->num_sys++;

  if(def->iter){
     SystemRegisterIter(def->name);
    if(def->num_req > 0)
      SystemRegisterIter(def->components[0]);
  }
  
  for (int i = 0; i < UPDATE_DONE; i++){
    if(!def->syncs[i])
      continue;

    s->tick[i] = def->syncs[i];
    uint64_t n = GameEvent_ToNotif(GAME_EVENT_SYNC);
    TargetSubscribe(n, OnSystemEvent, s, i);
  }

  for (int i = 0; i < UPDATE_DONE; i++){
    if(!def->steps[i])
      continue;
  
    s->step[i] = def->steps[i];
    uint64_t n = GameEvent_ToNotif(GAME_EVENT_STEP);
    TargetSubscribe(n, OnSystemEvent, s, i);
  }

  for (int i = 0; i < GAME_DONE; i++){
    if(!def->sets[i])
      continue;

    s->set[i] = def->sets[i];
    uint64_t n = GameEvent_ToNotif(GAME_EVENT_SET);
    TargetSubscribe(n, OnSystemEvent, s, i);
  }

  for (int i = 0; i < GAME_DONE; i++){
    if(!def->states[i])
      continue;

    s->state[i] = def->states[i];
    uint64_t n = GameEvent_ToNotif(GAME_EVENT_STATE);
    TargetSubscribe(n, OnSystemEvent, s, i);
  }

  for (int i = 0; i < def->num_req; i++){
    comp_id_t cid = ComponentGetID(def->components[i]);
    if(cid == INVALID_COMPONENT){
      TraceLog(LOG_ERROR, "=== SYSTEM CREATE ===\n System %s invalid required Component %s",
          def->name, def->components[i]);
      exit;
    }
    SystemRequire(s, cid);

  }
  WorldMapSystem(w, def->name, s);
  return s;
}
