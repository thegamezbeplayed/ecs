#ifndef __GAME_COMP__
#define __GAME_COMP__

#include "screens.h"
#include "game_physics.h"
#include "game_behaviors.h"
#include "game_assets.h"
#include "game_control.h"

#define MAX 1024
#define MAX_COMPONENTS 512
#define MAX_PLAYERS 2

#define FIRST(pool) \
    ((pool).size == 0 ? -1 : (pool).entities[0])

typedef struct{
  Entity  entities[MAX_COMPONENTS];
  int     sparse[MAX_ENTITIES];
  size_t  size;
}component_t;

static int ComponentGetFirst(int* entities, size_t size){
    if (size == 0) return -1; // invalid
    return entities[0];
}

typedef struct{
  rigid_body_t    dense[MAX_COMPONENTS];
  component_t     map;
}rigid_body_c;

typedef struct{
  behavior_t    dense[MAX_COMPONENTS];
  component_t   map;
}behavior_c;

typedef struct{
  sprite_t    dense[MAX_COMPONENTS];
  component_t   map;
}sprite_c;

typedef struct{
  camera_t    dense[MAX_CAMERA];
  component_t map;
}camera_c;

typedef struct{
  camera_ctx_t  dense[MAX_CAMERA];
  component_t   map;
}camera_follow_c;

typedef struct{
  input_t   dense[MAX_PLAYERS];
  component_t    map;
}input_c;

static void ComponentInit(component_t* c){
  c->size = 0;
}

static void CameraInit(camera_c* c){
  ComponentInit(&c->map);
}

static void SpriteInit(sprite_c* c){
  ComponentInit(&c->map);
}

#endif
