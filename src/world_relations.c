#include "world_define.h"

hash_map_t ENT_RELATIONS;

uint32_t RelationBitRegister(world_t* w, const char* str){
  RelationId id = XXH3_64bits(str, strlen(str));

  uint32_t* existing = HashGet(&w->relation_to_bit, id);
  if (existing)
    return *existing;

  uint32_t* bit = GameCalloc("RelationBitRegister", 1, sizeof(uint32_t));
  *bit = 1u << w->next_relation_bit++;
  HashPut(&w->relation_to_bit, id, bit);

  TraceLog(LOG_INFO, "=== Register New Relation %s at bit %i", str, *bit);
  return *bit;
}

uint32_t RelationGetBitById(world_t* w, RelationId id)
{
    uint32_t* bit = HashGet(&w->relation_to_bit, id);
    return bit ? *bit : 0;
}

uint32_t RelationGetBit(world_t* w, const char* name)
{
    if (!name || !*name) return 0;
    RelationId id = XXH3_64bits(name, strlen(name));
    return RelationGetBitById(w, id);
}

void InitRelationMap(int cap){
  return HashInit(&ENT_RELATIONS, next_pow2_int(cap));
}

void EntityMapRelation(world_t* w, Entity e, relation_t* r){
  uint32_t bit = RelationBitRegister(w, r->type);
  uint32_t fnv = hash_string_fnv(r->type);
  hash_key_t key = hash_64_combine(e.id, fnv);
  w->entity_relation_mask[e.id] |= bit;
  HashPut(&ENT_RELATIONS, key, r);
}

relation_t* EntityAddRelation(world_t* w, Entity e, const char* type, Entity target){
  if (!EntityValid(&w->manager, target))
    return NULL;

  relation_t* r = GameCalloc("EntityAddRelation", 1, sizeof(relation_t));

  r->target = target;
  strcpy(r->type, type);

  EntityMapRelation(w, e, r);
  return r;
} 
  
void EntityRemoveRelation(world_t* w, Entity e){
  if (e.id >= MAX_ENTITIES) return;
  //w->has_relation[e.id] = false;
}

relation_t* EntityGetRelation(world_t* w, Entity e, const char* type){
  uint32_t fnv = hash_string_fnv(type);
  hash_key_t key = hash_64_combine(e.id, fnv);
  
  return HashGet(&ENT_RELATIONS, key);
}

Entity EntityGetRelationTarget(world_t* w, Entity e, const char* rel){
  if (!EntityValid(&w->manager, e))
    return INVALID_ENTITY; 

  relation_t* r = EntityGetRelation(w, e, rel);

  if(!r)
    return INVALID_ENTITY;

  return r->target; 
}

bool EntityHasRelation(world_t* w, Entity e, const char* rel){
  uint32_t bit = RelationGetBit(w, rel);

  return (w->entity_relation_mask[e.id] & bit) == bit;

}
