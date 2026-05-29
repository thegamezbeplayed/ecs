#ifndef __UTIL_HASH__
#define __UTIL_HASH__
#include "xxhash.h" 
#include "util_tools.h"

#define HKEY_CELL(c) (hash_key_t){hash_combine_64(\
    hash_64_from_int(c.x), hash_64_from_int(c.y))}

typedef uint64_t hash_key_t;

typedef struct{
  uint32_t  id;
  Vector2   pos;
}neighbor_t;

typedef struct {
  neighbor_t* items;
  int         count;
  int         capacity;
} neighbor_list_t;

typedef struct spacial_hash_entry_s spacial_hash_entry_t;
struct spacial_hash_entry_s{
  uint32_t  id;
  Vector2   pos;
  float     radius;
  spacial_hash_entry_t* next;
};

typedef struct{
  float                     cell_size;
  uint32_t                  num_buckets;
  spacial_hash_entry_t**    buckets;
}spacial_hash_grid_t;
spacial_hash_grid_t* SpatialHashGrid(float cell_size, uint32_t count);
void SpatialHashGridDestroy(spacial_hash_grid_t* shg);
void SpatialHashGridClear(spacial_hash_grid_t* shg);

Cell SpatialHashGridInsert(spacial_hash_grid_t*, uint32_t, Vector2, float);
neighbor_list_t SpatialHashGridGetNeighbors(const spacial_hash_grid_t*, Vector2, float,uint32_t self_id);
typedef struct {
    hash_key_t key;
    void* value;
    uint8_t state; // 0 = empty, 1 = used, 2 = tombstone
} hash_slot_t;

typedef struct {
    hash_slot_t* slots;
    uint32_t cap;
    uint32_t count;
} hash_map_t;

static bool HashFull(hash_map_t* m){
 return (m->count > m->cap/2);

}

typedef struct{
  hash_map_t* map;
  uint32_t index;
}hash_iter_t;
static void HashStart(hash_map_t* m, hash_iter_t* it) {
    it->map = m;
    it->index = 0;
}

static hash_slot_t* HashNext(hash_iter_t* it) {
    hash_map_t* m = it->map;

    while (it->index < m->cap) {
        hash_slot_t* s = &m->slots[it->index++];

        if (s->state == 1)
            return s;

        if (s->state == 2)
          continue;
    }

    return NULL;
}

void HashInit(hash_map_t* m, uint32_t cap);
void HashFree(hash_map_t* m);
void HashClear(hash_map_t* m);
hash_key_t HashKey(hash_map_t* m, hash_key_t key);
void* HashGet(hash_map_t* m, hash_key_t key);
void HashPut(hash_map_t* m, hash_key_t key, void* value);
void HashRemove(hash_map_t* m, hash_key_t key);
void HashExpand(hash_map_t* m);

static inline hash_key_t hash_pair_32(uint32_t a, uint32_t b) {
    normalize_32(&a, &b);

    hash_key_t h = 1469598103934665603ULL;

    h ^= a;
    h *= 1099511628211ULL;

    h ^= b;
    h *= 1099511628211ULL;

    return h;
}

static inline uint64_t hash_event(uint32_t a, uint32_t b, uint64_t type){
  if (a > b) { uint32_t t = a; a = b; b = t; }

    uint64_t h = 1469598103934665603ULL;

    h ^= (uint64_t)a; h *= 1099511628211ULL;
    h ^= (uint64_t)b; h *= 1099511628211ULL;
    h ^= type;         h *= 1099511628211ULL;

    return h;
}

static inline uint64_t hash_64(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static uint64_t hash_64_from_int(int x)
{
    uint64_t z = (uint64_t)x + 0x9E3779B97F4A7C15ULL;

    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z = z ^ (z >> 31);

    return z;
}
static int int_from_hash_64(uint64_t h)
{
    h ^= h >> 32;
    return (int)h;                    // simple and fast
}
static uint32_t hash_float(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(float)); // safe bit copy

    // simple mix (you can improve this)
    bits ^= bits >> 16;
    bits *= 0x7feb352d;
    bits ^= bits >> 15;
    bits *= 0x846ca68b;
    bits ^= bits >> 16;
    
    return bits;
}   
static inline uint64_t hash_64_combine(uint32_t a, uint32_t b)
{
    uint64_t combined = ((uint64_t)a << 32) | b;
    // Simple but effective mixing
    combined ^= combined >> 33;
    combined *= 0xff51afd7ed558ccdull;   // 64-bit murmur constant
    combined ^= combined >> 33;
    return combined;
}

static inline uint64_t hash_combine_64(uint64_t h, uint64_t v) {
    h ^= v + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
    return h;
}

static inline uint32_t hash_combine_32(uint32_t h, uint32_t v)
{
    return h ^ (v + 0x9e3779b9 + (h << 6) + (h >> 2));
}

static uint32_t hash_str_32(const char *str) {
  if(!str)
    return 0;

  uint32_t hash = 5381; // djb2 starting seed
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + (uint32_t)c; // hash * 33 + c
  return hash;
}

static uint32_t hash_string_fnv(const char* str){
  uint32_t hash = 0x811C9DC5u;
  while(*str){
    hash ^= (uint8_t)*str++;
    hash *= 0x01000193u;
  }

  return hash;
}

static inline uint64_t hash_str_64(const char* s) {
    uint64_t h = 1469598103934665603ULL; // FNV offset basis
    while (*s) {
        h ^= (uint8_t)(*s++);
        h *= 1099511628211ULL; // FNV prime
    }
    return h;
}       



#endif
