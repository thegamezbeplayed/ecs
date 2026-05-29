#include "util_hash.h"

void HashInit(hash_map_t* m, uint32_t cap) {
  assert((cap & (cap - 1)) == 0); // power of two
  m->cap = cap;
  m->count = 0;
  m->slots = GameCalloc("HashInit", cap, sizeof(hash_slot_t));
}

void HashClear(hash_map_t* m){
  GameFree("HashClear", m->slots);
  m->count = 0;
}

void HashFree(hash_map_t* m) {
  GameFree("HashFree", m->slots);
  memset(m, 0, sizeof(*m));
}

hash_key_t HashKey(hash_map_t* m, hash_key_t key) {
  uint32_t mask = m->cap - 1;
  uint64_t h = hash_64(key);

  for (uint32_t i = 0; i < m->cap; i++) {
    hash_slot_t* s = &m->slots[(h + i) & mask];

    if (s->state == 0)
      return 0;

    if (s->state == 1 && s->key == key)
      return s->key;
  }

  return 0;

}

void* HashGet(hash_map_t* m, hash_key_t key) {
  uint64_t h = hash_64(key);
  uint32_t mask = m->cap - 1;

  for (uint32_t i = 0; i < m->cap; i++) {
    hash_slot_t* s = &m->slots[(h + i) & mask];
    if (s->state == 0)
      return NULL;

    if (s->state == 1 && s->key == key)
      return s->value;
  }

  return NULL;
}
void HashExpand(hash_map_t* m){
  int old_cap = m->cap;
  size_t new_cap = old_cap * 2;
  hash_slot_t* new_entries = GameCalloc("HashExpand", new_cap, sizeof(hash_slot_t));

  for (int i = 0; i < old_cap; i++) {
    hash_slot_t* e = &m->slots[i];

    if (e->state != 1)
      continue;

    uint64_t h = hash_64(e->key);
    uint32_t mask = new_cap - 1;

    size_t idx = h  & mask;

    while (new_entries[idx].state == 1)
      idx = (idx + 1) & mask;

    new_entries[idx].key   = e->key;
    new_entries[idx].value = e->value;
    new_entries[idx].state = 1;
  }

  free(m->slots);
  m->slots = new_entries;
  m->cap = new_cap;
}

void HashPut(hash_map_t* m, hash_key_t key, void* value) {
  if(m->count * 4 > m->cap*3) // load factor < 0.5
    return;

  uint64_t h = hash_64(key);
  uint32_t mask = m->cap - 1;
  hash_slot_t* tomb = NULL;

  for (uint32_t i = 0; i < m->cap; i++) {
    hash_slot_t* s = &m->slots[(h + i) & mask];

    if (s->state == 1 && s->key == key) {
      s->value = value;
      return;
    }

    if (s->state == 2 && !tomb)
      tomb = s;

    if (s->state == 0) {
      s = tomb ? tomb : s;
      s->key = key;
      s->value = value;
      s->state = 1;
      m->count++;
      return;
    }
  }

  assert(0 && "HashPut failed");
}

void HashRemove(hash_map_t* m, hash_key_t key) {
  uint64_t h = hash_64(key);
  uint32_t mask = m->cap - 1;

  for (uint32_t i = 0; i < m->cap; i++) {
    hash_slot_t* s = &m->slots[(h + i) & mask];

    if (s->state == 0)
      return;

    if (s->state == 1 && s->key == key) {
      s->state = 2; // tombstone
      s->value = NULL;
      m->count--;
      return;
    }
  }
}

static uint32_t hash_cell(int32_t x, int32_t y, uint32_t num_buckets)
{
  uint64_t key = ((uint64_t)x << 32) | (uint32_t)y;
  key = (key ^ (key >> 30)) * 0xBF58476D1CE4E5B9ULL;
  key = (key ^ (key >> 27)) * 0x94D049BB133111EBULL;
  key = key ^ (key >> 31);
  return (uint32_t)(key % num_buckets);
}

spacial_hash_grid_t* SpatialHashGrid(float cell_size, uint32_t count){
  spacial_hash_grid_t* shg = GameMalloc("SpatialHashGrid", sizeof(spacial_hash_grid_t));
  shg->cell_size   = cell_size;
  shg->num_buckets = count;
  shg->buckets = GameCalloc("SpatialHashGrid", count,
      sizeof(spacial_hash_entry_t*));

  return shg;
}

void SpatialHashGridDestroy(spacial_hash_grid_t* shg){
  if (!shg) return;
  SpatialHashGridClear(shg);
  GameFree("SpatialHashGridDestroy", shg->buckets);
  GameFree("SpatialHashGridDestroy", shg);
}

void SpatialHashGridClear(spacial_hash_grid_t* shg){
  if (!shg) return;

  for (uint32_t i = 0; i < shg->num_buckets; ++i) {
    spacial_hash_entry_t* e = shg->buckets[i];
    while (e) {
      spacial_hash_entry_t* next = e->next;
      GameFree("SpatialHashGridClear", e);
      e = next;
    }
    shg->buckets[i] = NULL;
  }
}

Cell SpatialHashGridInsert(spacial_hash_grid_t* shg,
    uint32_t eid, Vector2 pos, float rad){
  if (!shg) return CELL_UNSET;
  // === Calculate primary / majority cell (center-based) ===
  int32_t main_x = (int32_t)floorf(pos.x / shg->cell_size);
  int32_t main_y = (int32_t)floorf(pos.y / shg->cell_size);

  int32_t min_x = (int32_t)floorf((pos.x - rad) / shg->cell_size);
  int32_t min_y = (int32_t)floorf((pos.y - rad) / shg->cell_size);
  int32_t max_x = (int32_t)floorf((pos.x + rad) / shg->cell_size);
  int32_t max_y = (int32_t)floorf((pos.y + rad) / shg->cell_size);
  Cell primary = { .x = main_x, .y = main_y };
  
  for (int32_t y = min_y; y <= max_y; ++y) {
    for (int32_t x = min_x; x <= max_x; ++x) {
      uint32_t bucket = hash_cell(x, y, shg->num_buckets);

      spacial_hash_entry_t* entry = GameMalloc("SpatialHashGridInsert",
          sizeof(spacial_hash_entry_t));

      entry->pos        = pos;
      entry->radius     = rad;
      entry->id         = eid;
      entry->next       = shg->buckets[bucket];
      shg->buckets[bucket] = entry;
    }
  }

  return primary;
}

void NeighborListInit(neighbor_list_t* list, int capacity) {
  list->items = GameMalloc("NeighborListInit", sizeof(neighbor_t) * capacity);
  list->count = 0;
  list->capacity = capacity;
}

void NeighborListFree(neighbor_list_t* list) {
  GameFree("NeighborListFree", list->items);
  list->items = NULL;
  list->count = 0;
  list->capacity = 0;
}

static inline void NeighborListPush(neighbor_list_t* list, uint32_t id, Vector2 pos) {
  if (list->count >= list->capacity) return; // or assert

  list->items[list->count++] = (neighbor_t){
    .id = id,
      .pos = pos
  };
}

neighbor_list_t SpatialHashGridGetNeighbors(const spacial_hash_grid_t* shg,
    Vector2 pos, float query_radius, uint32_t self_id){
  neighbor_list_t result;
  NeighborListInit(&result, 64); // or tune this
  if (!shg) 
    return result;
 
  float radius = (query_radius > 0.0f) ? query_radius : 100.0f;

  int32_t min_x = (int32_t)floorf((pos.x - radius) / shg->cell_size);
  int32_t min_y = (int32_t)floorf((pos.y - radius) / shg->cell_size);
  int32_t max_x = (int32_t)floorf((pos.x + radius) / shg->cell_size);
  int32_t max_y = (int32_t)floorf((pos.y + radius) / shg->cell_size);

  for (int32_t y = min_y; y <= max_y; ++y) {
    for (int32_t x = min_x; x <= max_x; ++x) {

      uint32_t bucket = hash_cell(x, y, shg->num_buckets);
      spacial_hash_entry_t* e = shg->buckets[bucket];

      while (e) {
        spacial_hash_entry_t* next = e->next;

        if (e->id != self_id) {
          NeighborListPush(&result, e->id, e->pos);
        }

        e = next;
      }
    }
  }

  return result;

}
