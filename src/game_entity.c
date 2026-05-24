#include "game_register.h"

uint32_t PLAYER;

entity_query_t EQ;

void EntityInit(EntityManager* em) {
  em->free_count = MAX_ENTITIES;

  for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
    em->free_list[i] = MAX_ENTITIES - i - 1;
    em->generation[i] = 0;
    em->alive[i] = 0;
  }
}

Entity EntityCreatePrefab(EntityManager* em) {
  uint32_t id = em->free_list[--em->free_count];

  Entity e = {
    .id = id,
    .generation = em->generation[id]
  };

  return e;
}

Entity EntityCreate(EntityManager* em) {
  uint32_t id = em->free_list[--em->free_count];

  em->alive[id] = 1;

  Entity e = {
    .id = id,
    .generation = em->generation[id]
  };

  return e;
}

void EntiyBatchReserve(EntityManager* m, int count, Entity pool[count]){
  for(int i = 0; i < count; i++)
    pool[i] = EntityCreate(m);
}

void EntityDestroy(EntityManager* em, Entity e) {
  if (!em->alive[e.id]) return;

  em->alive[e.id] = 0;

  em->generation[e.id]++;   // invalidate old references

  em->free_list[em->free_count++] = e.id;
}

void EntityRelationEnd(world_t* w, Entity e){
  EntityRemoveRelation(w, e);

  EntityDestroy(&w->manager, e);
}

bool EntityReady(EntityManager* em, Entity e) {
  return em->alive[e.id] &&
    em->generation[e.id] == e.generation;
}

bool EntityValid(EntityManager* em, Entity e) {
  if(e.id == INVALID_ENTITY.id)
    return false;

  return em->generation[e.id] == e.generation;
}

Entity EntityGet(EntityManager* em, uint32_t id)
{
  if (id >= MAX_ENTITIES)
    return INVALID_ENTITY;

  return (Entity){
    .id = id,
      .generation = em->generation[id]
  };
}

void QueryBegin(void){
  EQ.query_pos = 0;
  EQ.num_results = 0;
}

int QueryEntityByComp(world_t* w, int num_terms, comp_id_t terms[num_terms]){
  for (int i = 0; i < num_terms; i++){
    component_pool_t* pool = w->pools[terms[i]];
    if(!pool)
      return -1;
    EQ.terms[EQ.term_count++] = terms[i];
  }

  for(int i = 0; i < MAX_ENTITIES && EQ.num_results < MAX_QUERY_SET; i++){
    bool has = true;
    Entity e;
    for(int j = 0; j < EQ.term_count; j++){
      component_pool_t* pool = w->pools[EQ.terms[j]]; 
      if (pool->entities[i] == 0) {
        has = false;
        break;
      }

      e = EntityGet(&w->manager, pool->entities[i]);

      if (!EntityReady(&w->manager, e))
        has = false;
    }

    if(!has)
      continue;

    EQ.results[EQ.num_results++] = e.id;

  }

  return EQ.num_results;
}

Entity QueryGetNext(world_t* w){
  if(EQ.num_results == 0 || EQ.query_pos >= EQ.num_results)
    return INVALID_ENTITY;

  return EntityGet(&w->manager, EQ.results[EQ.query_pos++]);
}

entity_iter_t* EntityIterInit(world_t* w, system_t* s){
  entity_iter_t* it = GameCalloc("EntityIterInit", 1, sizeof(entity_iter_t));
  it->terms = s->terms;
  it->term_count = s->term_count;

  it->base = ComponentQueryInner(w, s);
  if (!it->base)
    return it;

  it->index = -1;   // will be incremented on first Next()
  return it;
}

bool EntityIterStart(world_t* w, entity_iter_t* it, system_t* s){
  it->terms = s->terms;
  it->term_count = s->term_count;

  it->base = ComponentQueryInner(w, s);
  it->index = -1;   // will be incremented on first Next()

  return it->base != NULL;
}

bool EntityIterNext(entity_iter_t* it, world_t* w){
  if (!it->base)
    return false;

  while (++it->index < it->base->size)
  {
    uint32_t id = it->base->entities[it->index];
    Entity e = { id, w->manager.generation[id] };

    if (!EntityReady(&w->manager, e))
      continue;

    // Check if entity has ALL required components
    bool match = true;
    for (int t = 0; t < it->term_count; ++t)
    {
      component_pool_t* p = w->pools[it->terms[t]];
      if (p->sparse[id] == -1)   // doesn't have this component
      {
        match = false;
        break;
      }
    }

    if (match)
    {
      it->current = e;
      return true;
    }
  }

  return false;
}
