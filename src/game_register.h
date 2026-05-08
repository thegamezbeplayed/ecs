#ifndef __GAME_REG__
#define __GAME_REG__

#include "game_components.h"
#include "game_process.h"
#include "game_define.h"

#define MAX_TERMS 8
#define MAX_PREFABS 128
#define MAX_COMP_DATA 8

#define ADD_COMPONENT(world, e, Type, ID) \
  (Type*)ComponentAdd(world, e, ID)

#define GET_COMPONENT(world, e, Type, ID) \
  (Type*)ComponentGet(world, e, ID)

#define REGISTER_COMPONENT(world, Type) \
  ComponentRegister(world, sizeof(Type))

#define MAX_RELATIONS_PER_ENTITY 4 
#define MAX_RELATIONS   8
#define REL_AppliesTo   1u
#define REL_ChildOf     2u          
#define REL_Owner       3u         
#define REL_Target      4u

typedef uint32_t RelationType;
typedef struct {
    Entity       target;      // INVALID_ENTITY = no relation
    RelationType type;
} relation_t;

typedef struct world_s world_t;
void EntityAddRelation(world_t*, Entity, RelationType, Entity);
void EntityRemoveRelation(world_t*, Entity);
Entity EntityGetRelationTarget(world_t*, Entity, RelationType);
bool EntityHasRelation(world_t*, Entity, RelationType);

void EntityRelationEnd(world_t*, Entity);

typedef void (*SystemCB)(world_t* w, Entity e);
typedef void (*SystemFn)(world_t* w);

typedef struct {
  int       index;
  comp_id_t terms[MAX_TERMS];
  int       term_count;
  SystemFn  init;
  SystemCB  set[GAME_DONE];
  SystemCB  tick[UPDATE_DONE];
  bool      needs_iter;
} system_t;

system_t* SystemRegister(world_t* w, SystemCB*, SystemCB*, SystemFn);

void SystemTick(world_t* w, system_t* s, UpdateType u);
void SystemSet(world_t* w, system_t* s, GameState g);

static void SystemRequire(system_t* s, comp_id_t id) {
    s->terms[s->term_count++] = id;
}

typedef struct {
  comp_id_t  id;

  int        entities[MAX_ENTITIES];
  int        sparse[MAX_ENTITIES];
  size_t     size;

  size_t            elem_size;   // size of component (Position, etc)
  void*             data;        // dense array of component data
} component_pool_t;
bool HasComponent(component_pool_t* pool, Entity e);

typedef struct {
    char          name[64];
    Entity        entity;        // the template entity
    comp_id_t     components[MAX_TERMS];  // list of components it has
    int           comp_count;
} prefab_t;

typedef struct {
    prefab_t prefabs[MAX_PREFABS];
    int      count;
} prefab_registry_t;

typedef struct{
  component_pool_t*   base;
  int                 index, term_count;
  Entity              current;
  const comp_id_t*    terms;
}entity_iter_t;

struct world_s {
  EntityManager     manager;

  component_pool_t* pools[MAX_COMPONENTS];
  uint32_t          next_component_id;

  int               num_sys;
  system_t*         systems;
  prefab_registry_t prefabs;

  relation_t        relations[MAX_ENTITIES];
  bool              has_relation[MAX_ENTITIES];  
  entity_iter_t*    iter; 
};
extern world_t world;

static void EntityIterReset(entity_iter_t* it){
  it->index = -1;
}
entity_iter_t EntityIterStart(world_t*, system_t*);
bool EntityIterNext(entity_iter_t*, world_t*);

component_pool_t* ComponentQueryInner(world_t* w, system_t* s);
void WorldInit(world_t* w, int sys_cap);
void* ComponentAdd(world_t* w, Entity e, comp_id_t id);
void* ComponentGet(world_t* w, Entity e, comp_id_t id);
comp_id_t ComponentRegister(world_t* w, size_t);

void RegisterComponentData(world_t* w);
void RegisterSystemData(world_t* w);
void RegisterEntityData(world_t* w);

// Add to world_t
void PrefabRegistryInit(world_t* w);
Entity PrefabCreate(world_t* w, const char* name);
Entity PrefabInstantiate(world_t* w, Entity prefab, Vector2 override_pos); // basic override
Entity PrefabInstantiateFull(world_t* w, const char* name, Vector2 pos, ...); // later
prefab_t* PrefabFind(world_t* w, const char* name);
Entity PrefabSpawn(world_t* w, const char* name, Vector2 world_pos);

typedef struct{
  int       query_pos, num_results;
  uint32_t  results[MAX_QUERY_SET];
  comp_id_t terms[MAX_TERMS];
  int       term_count;
}entity_query_t;
extern entity_query_t EQ;
void QueryBegin(void);
int QueryEntityByComp(world_t* w, int, comp_id_t[]);
Entity QueryGetNext(world_t* w);

#endif
