#ifndef __GAME_COMMON__
#define __GAME_COMMON__

#include "raylib.h"
#include "game_types.h"
#include "game_enum.h"

#define MAX_ENTITIES 640

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
void EntityTest(int count);
bool EntityValid(EntityManager* em, Entity e);

/*
typedef bool (*StateComparator)(int a, int b);

typedef struct{
  int             state;
  StateComparator can;
  int             required;
}state_change_requirement_t;

static state_change_requirement_t CAN_CHANGE[STATE_END+1] = {
  {STATE_NONE, NEVER, STATE_END},
  {STATE_SPAWN, LESS_THAN, STATE_SPAWN},
  {STATE_IDLE, LESS_THAN, STATE_DIE},
  {STATE_DIE, LESS_THAN, STATE_DIE},
  {STATE_END, EQUAL_TO, STATE_DIE},
};
*/
#endif
