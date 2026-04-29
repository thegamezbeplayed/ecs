#ifndef __GAME_REG__
#define __GAME_REG__

#include "game_components.h"
#include "game_tools.h"

typedef struct{
  camera_c        cam;
  camera_follow_c view;
  EntityManager   manager;
  behavior_c      behaviors;
  sprite_c        sprites;
  rigid_body_c    bodies;
  state_c         states;
  input_c         input;
  anim_c          anim;
  position_c      pos;
}component_registry_t;

bool HasSprite(sprite_c* c, Entity e);
extern component_registry_t world;

void InitComponents(void);

static sprite_t* SpriteGet(sprite_c* c, Entity e) {
    return &c->dense[c->map.sparse[e.id]];
}

void RegisterState(state_c*, Entity, state_t);
void RegisterBehavior(behavior_c*, Entity, behavior_t);
void RegisterRigidBody(rigid_body_c*, Entity, rigid_body_t);
void RegisterPos(position_c* c, Entity e, position_t pos);
void RegisterSprite(sprite_c* c, Entity e, sprite_t s);
void RegisterAnim(anim_c* c, Entity e, anim_player_t a);
void RegisterInput(input_c* c, Entity e, input_t s);
void RegisterCamera(camera_c* c, Entity e, camera_t s);
void RegisterCameraFollow(camera_follow_c* c, Entity e, camera_ctx_t s);
static camera_t* CameraGetActive(camera_c* c){
  return &c->dense[0];//FIRST(c->map)];
}
#endif
