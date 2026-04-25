#ifndef __GAME_REG__
#define __GAME_REG__

#include "game_components.h"
#include "game_tools.h"

typedef struct{
  camera_c        cam;
  camera_follow_c view;
  EntityManager   manager;
  rigid_body_c    rigid_bodies;
  behavior_c      behaviorS;
  sprite_c        sprites;
  input_c         input;
}component_registry_t;

bool HasSprite(sprite_c* c, Entity e);
extern component_registry_t world;

void InitComponents(void);

static sprite_t* SpriteGet(sprite_c* c, Entity e) {
    return &c->dense[c->map.sparse[e.id]];
}

void RegisterSprite(sprite_c* c, Entity e, sprite_t s);
void RegisterInput(input_c* c, Entity e, input_t s);
void RegisterCamera(camera_c* c, Entity e, camera_t s);
void RegisterCameraFollow(camera_follow_c* c, Entity e, camera_ctx_t s);
static camera_t* CameraGetActive(camera_c* c){
  return &c->dense[0];//FIRST(c->map)];
}
#endif
