#ifndef __COMP_DEF__
#define __COMP_DEF__

#include "asset_vfx.h"
#include "physics_define.h"
#include "view_define.h"
#include "game_stats.h"
#include "game_behaviors.h"
#include "game_control.h"

bool InputInit(void* comp, component_entry_t* j);
bool SpriteInit(void* comp, component_entry_t* j);
bool RenderContextInit(void* comp, component_entry_t* j);
bool ForceInit(void* comp, component_entry_t* j);
bool RigidBodyInit(void* comp, component_entry_t* j);
bool AnimInit(void* comp, component_entry_t* j);
bool CameraInit(void* comp, component_entry_t* j);
bool PositionInit(void* comp, component_entry_t* j);
bool CoordInit(void* comp, component_entry_t* j);
bool ParticleInit(void* comp, component_entry_t* j);
bool ParticleEmitterInit(void* comp, component_entry_t* j);
bool ObserverInit(void* comp, component_entry_t* j);
bool SubjectInit(void* comp, component_entry_t* j);
bool TrackingInit(void* comp, component_entry_t* j);


bool ParseSpriteComponent(cJSON* j, sprite_t* out);
bool ParseRigidBodyComponent(cJSON* j, rigid_body_t* out);
bool ParseForceComponent(cJSON* j, force_t* out);
bool ParsePositionComponent(cJSON* j, position_t* out);
bool ParseInputComponent(cJSON* j, input_t* out);
bool ParseCameraComponent(cJSON* j, camera_t* out);
bool ParseParticleEmitterComponent(cJSON*, particle_emitter_t*);
bool ParseRenderComponent(cJSON* j, render_ctx_t* out);
bool ParseObserverComponent(cJSON* j, component_observer_t* out);
bool ParseSubjectComponent(cJSON* j, subject_component_t* out);

typedef struct{
  anim_player_t   player;
  AnimEventID     event;
  int             num_hurt;
  collision_d     hitbox;
  collision_d     hurtboxes[MAX_SLICES];
  anim_t          sequences[ANIM_DONE][MAX_DIRECTIONS];
}anim_comp_t;
bool ParseAnimComponent(cJSON* j, anim_comp_t* out);

typedef struct{
  char  name[MAX_NAME_LEN];
}name_t;

typedef struct{
  int       wid, hei;
  int       num_tiles;
  tile_t*   tiles;
  Rectangle bounds;
}level_t;


typedef struct{
  camera_ctx_t      ctx;
  uint32_t          target;
}tracking_t;
bool ParseTrackingComponent(cJSON* j, tracking_t* out);

typedef struct{
  bool    assigned;
}follow_t;
static bool FollowInit(void* comp, component_entry_t* data){ return true;}

typedef struct{

}ai_comp_t;

typedef struct{
  State   state, last;
}state_comp_t;

typedef struct{
  int   duration;
  int   expiration;
}lifetime_t;

static void LifetimeSet(lifetime_t* lf, int dur){
 lf->expiration = WorldGetTime() + dur;
}
#endif
