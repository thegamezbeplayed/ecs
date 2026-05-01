#include "game_register.h"

uint32_t PLAYER;

void EntityInit(EntityManager* em) {
  em->free_count = MAX_ENTITIES;

  for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
    em->free_list[i] = MAX_ENTITIES - i - 1;
    em->generation[i] = 0;
    em->alive[i] = 0;
  }
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

void EntityDestroy(EntityManager* em, Entity e) {
  if (!em->alive[e.id]) return;

  em->alive[e.id] = 0;

  em->generation[e.id]++;   // invalidate old references

  em->free_list[em->free_count++] = e.id;
}

bool EntityValid(EntityManager* em, Entity e) {
  return em->alive[e.id] &&
    em->generation[e.id] == e.generation;
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
