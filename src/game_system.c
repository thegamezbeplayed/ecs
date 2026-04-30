#include "game_register.h"

system_t* SystemRegister(world_t* w, int ticks, int sets, SystemCB tick[ticks], SystemCB set[sets]){
  system_t* s = &w->systems[w->num_sys++];
  memset(s, 0, sizeof(system_t));
  for (int i = 0; i < ticks; i++)
    s->tick[i] = tick[i];

  for (int i = 0; i < sets; i++)
    s->set[i] = set[i];

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
