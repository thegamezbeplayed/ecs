#include "game_define.h"

<<<<<<< HEAD
comp_id_t INVALID_COMPONENT = -1;
=======
static comp_id_t INVALID_COMPONENT = 0;
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

static hash_map_t COMP_REGISTER;

static int CompCount = 0;

void ComponentInit(int count)
{
  HashInit(&COMP_REGISTER, next_pow2_int(count*2));
}

void ComponentRegisterCore(const char* name)
{
  for(int i = 0; i < NUM_COMP_CORE; i++){
    component_define_t def = CORE_COMPONENTS[i];
    if(strcmp(name, def.name) == 0 && def.size > 0){
      ComponentRegister(&world, def.name, def.size);
      return;
    }
  }

  TraceLog(LOG_WARNING, "=== COMPONENT REGISTER CORE ====\n %s NOT FOUND!", name);
}

comp_id_t ComponentGetID(const char* name)
{
    if (!name) return 0;

    uint64_t hash = hash_str_64(name);

    component_t* exists = HashGet(&COMP_REGISTER, hash);

    if(exists)
      return exists->id;

    return INVALID_COMPONENT;
}

comp_id_t ComponentRegister(world_t* w, const char* name, size_t elem_size){

  comp_id_t exists = ComponentGetID(name);
  if(exists != INVALID_COMPONENT)
    return exists;
  
  comp_id_t id = w->next_component_id++;

  component_pool_t* pool = GameCalloc("ComponentRegister", 1, sizeof(component_pool_t));

  pool->id = id;
  pool->elem_size = elem_size;
  pool->data = GameMalloc("ComponentRegister", elem_size * MAX_ENTITIES);

  // initialize sparse to -1 (meaning “not present”)
  for (int i = 0; i < MAX_ENTITIES; i++) {
    pool->sparse[i] = -1;
  }

  w->pools[id] = pool;

  component_t *comp = GameCalloc("ComponentRegister", 1, sizeof(component_t));
  comp->id = id;
  comp->name = name;

  uint64_t hash = hash_str_64(name);

  HashPut(&COMP_REGISTER, hash, comp);
  return id;
}

void* ComponentAdd(world_t* w, Entity e, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  int idx = pool->size++;

  pool->entities[idx] = e.id;
  pool->sparse[e.id] = idx;

  void* ptr = (char*)pool->data + (idx * pool->elem_size);

  memset(ptr, 0, pool->elem_size);

  return ptr;
}

void* ComponentGet(world_t* w, Entity e, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  if(!pool || id >= w->next_component_id)
    return NULL;

  int idx = pool->sparse[e.id];
  if (idx == -1) return NULL;

  return (char*)pool->data + (idx * pool->elem_size);
}

bool HasComponent(component_pool_t* pool, Entity e) {
    if (!pool) return false;
    if (!EntityValid(&world.manager, e)) return false;
    if (e.id >= MAX_ENTITIES) return false;

    int idx = pool->sparse[e.id];

    if (idx < 0) return false;

    return (pool->entities[idx] == e.id);

}

