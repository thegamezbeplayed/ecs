#ifndef __GBM_COMP__
#define __GBM_COMP__
#include "game_common.h"

#define NUM_COMP_CORE   18
#define MAX             1024
#define MAX_COMPONENTS  512
#define MAX_PLAYERS     2

#define POS_ID       ComponentGetID("Position")
#define PHYS_ID      ComponentGetID("RigidBody")
#define ANIM_ID      ComponentGetID("Animation")
#define SPR_ID       ComponentGetID("Sprite")
#define RENDER_ID    ComponentGetID("Render")
#define INPUT_ID     ComponentGetID("Input")
#define CAM_ID       ComponentGetID("Camera")
#define TRACK_ID     ComponentGetID("Track")
#define TYPE_ID      ComponentGetID("Type")
#define STAT_ID      ComponentGetID("Stat")
#define FORCE_ID     ComponentGetID("Force")
#define NAME_ID      ComponentGetID("Name")
#define STATE_ID     ComponentGetID("State")
#define FOLLOW_ID    ComponentGetID("Follow")
#define LVL_ID       ComponentGetID("Level")
#define EXPIR_ID     ComponentGetID("Expiry")
#define PARTICLE_ID  ComponentGetID("Particle")
#define EMITTER_ID   ComponentGetID("ParticleEmitter")
#define OBSERVE_ID   ComponentGetID("Observer")
#define SUBJECT_ID   ComponentGetID("Subject")

typedef uint64_t comp_id_t;
extern comp_id_t INVALID_COMPONENT;

typedef struct{
  const char*   name;
  size_t        size;
}component_define_t;

typedef struct{
  comp_id_t   id;
  const char* name;
}component_t;

void ComponentInit(int count);
comp_id_t ComponentGetID(const char* name);
void ComponentRegisterCore(const char* name);

typedef struct {
  comp_id_t   id;
  int         entities[MAX_ENTITIES];
  bool        has_update[MAX_ENTITIES];
  int         sparse[MAX_ENTITIES];
  size_t      size;

  size_t            elem_size;   // size of component (Position, etc)
  void*             data;        // dense array of component data
} component_pool_t;

bool HasComponent(component_pool_t* pool, Entity e);
#endif
