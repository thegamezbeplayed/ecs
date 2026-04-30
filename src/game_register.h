#ifndef __GAME_REG__
#define __GAME_REG__

#include "game_components.h"
#include "game_process.h"
#include "game_define.h"

#define MAX_TERMS 8

#define ADD_COMPONENT(world, e, Type, ID) \
  (Type*)ComponentAdd(world, e, ID)

#define GET_COMPONENT(world, e, Type, ID) \
  (Type*)ComponentGet(world, e, ID)

#define REGISTER_COMPONENT(world, Type) \
  ComponentRegister(world, sizeof(Type))

typedef struct world_s world_t;

typedef void (*SystemCB)(world_t* w, Entity e);

typedef struct {
  int       index;
  comp_id_t terms[MAX_TERMS];
  int       term_count;

  SystemCB  set[GAME_DONE];
  SystemCB  tick[UPDATE_DONE];
} system_t;

system_t* SystemRegister(world_t* w, SystemCB*, SystemCB*);

void SystemTick(world_t* w, system_t* s, UpdateType u);
void SystemSet(world_t* w, system_t* s, GameState g);

static void SystemRequire(system_t* s, comp_id_t id) {
    s->terms[s->term_count++] = id;
}

typedef struct {
  comp_id_t id;

  int entities[MAX_ENTITIES];
  int sparse[MAX_ENTITIES];
  size_t size;

  size_t elem_size;   // size of component (Position, etc)
  void*  data;        // dense array of component data
} component_pool_t;
struct world_s {
  EntityManager     manager;

  component_pool_t* pools[MAX_COMPONENTS];
  uint32_t          next_component_id;

  int               num_sys;
  system_t*         systems;
  //relation_index_t relations[MAX_RELATIONS];
};
extern world_t world;

component_pool_t* ComponentQueryInner(world_t* w, system_t* s);
void WorldInit(world_t* w, int sys_cap);
component_pool_t* StartComponentPool(world_t* w, comp_id_t id);
Entity EntityPair(world_t*, Entity r, Entity o);
void* ComponentAdd(world_t* w, Entity e, comp_id_t id);
void* ComponentGet(world_t* w, Entity e, comp_id_t id);
comp_id_t ComponentRegister(world_t* w, size_t);

void RegisterComponentData(world_t* w);
void RegisterSystemData(world_t* w);
void RegisterEntityData(world_t* w);

#endif
