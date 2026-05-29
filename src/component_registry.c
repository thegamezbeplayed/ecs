#include "game_define.h"

comp_id_t INVALID_COMPONENT = -1;
static hash_map_t COMP_REGISTER;

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
  pool->dirty = true;
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

void* ComponentGetByID(world_t* w, uint32_t eid, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  if(!pool || id >= w->next_component_id)
    return NULL;

  int idx = pool->sparse[eid];
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

void ComponentRemove(world_t* w, Entity e, comp_id_t id){
  component_pool_t* pool = w->pools[id];

  if(!pool || id >= w->next_component_id)
    return;

  pool->sparse[e.id] = -1;

  pool->dirty = true;
}

void ComponentsClear(world_t* w, Entity e) {
  if (!EntityValid(&w->manager, e)) return;

  for (int i = 0; i < w->next_component_id; ++i)   // GP = your global game state
  {
    ComponentRemove(w, e, i);
  }

}

bool ComponentCheck(world_t* w, comp_id_t cid, Entity e, notification n){
  component_pool_t* p = w->pools[cid];
  if(!p || cid >= w->next_component_id)
    return false;

  return (p->updates[e.id] & n) == n;
}

void ComponentUpdate(world_t* w, Entity e, comp_id_t cid, notification n){
  component_pool_t* p = w->pools[cid];
  if(!p || cid >= w->next_component_id)
    return;

  p->updated = true;
  p->updates[e.id] |= n;
}

void ComponentClearUpdate(world_t* w, Entity e, comp_id_t cid){
  component_pool_t* p = w->pools[cid];
  if(!p || cid >= w->next_component_id)
    return;

  p->updates[e.id] = 0;
}
