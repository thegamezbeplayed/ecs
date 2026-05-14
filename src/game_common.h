#ifndef __GAME_COMMON__
#define __GAME_COMMON__

#include "raylib.h"
#include "game_types.h"
#include "game_enum.h"

#define MAX_ENTITIES 8192
#define MAX_NAME_LEN 128
#define MAX_QUERY_SET 64
#define ENT_MAX_ADD  32
typedef struct {
    uint32_t id;
    uint32_t generation;
} Entity;

static const Entity INVALID_ENTITY = {UINT32_MAX, -1};

typedef struct {
    uint32_t generation[MAX_ENTITIES];
    uint8_t  alive[MAX_ENTITIES];

    uint32_t free_list[MAX_ENTITIES];
    uint32_t free_count;
} EntityManager;
void EntityInit(EntityManager* em);
Entity EntityCreate(EntityManager* em);
Entity EntityCreatePrefab(EntityManager* em);
void EntiyBatchReserve(EntityManager*, int count, Entity pool[count]);
bool EntityValid(EntityManager* em, Entity e);
bool EntityReady(EntityManager* em, Entity e);
Entity EntityGet(EntityManager* em, uint32_t id);
#endif
