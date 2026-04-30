#ifndef __GAME_COMMON__
#define __GAME_COMMON__

#include "raylib.h"
#include "game_types.h"
#include "game_enum.h"

#define MAX_ENTITIES 640
#define MAX_NAME_LEN 128

static uint32_t PLAYER;

typedef struct {
    uint32_t id;
    uint32_t generation;
} Entity;

typedef struct {
    uint32_t generation[MAX_ENTITIES];
    uint8_t  alive[MAX_ENTITIES];

    uint32_t free_list[MAX_ENTITIES];
    uint32_t free_count;
} EntityManager;
void EntityInit(EntityManager* em);
Entity EntityCreate(EntityManager* em);
bool EntityValid(EntityManager* em, Entity e);

#endif
