#ifndef __GAME_SYS__
#define __GAME_SYS__
#include "game_define.h"
#include "component_define.h"

#define NUM_FUNCTIONS   (sizeof(FUNCTION_LOOKUP) / sizeof(FUNCTION_LOOKUP[0]))

extern hash_map_t SYSTEM_SINK;
typedef struct{
  const char* name;
  ObserverCB  fn;
}system_sink_l;
void LookAddSink(const char* str, ObserverCB fn);
ObserverCB LookupSystemSink(const char* str);

system_t* SystemCreate(world_t* w, system_define_t* def);

void DebugSystem(world_t* w, Entity e);

void PositionLoad(world_t* w, Entity e);
void PositionPrep(world_t* w);
void PositionRegister(world_t* w);

void AnimLoad(world_t* w, Entity e);
void AnimReady(world_t* w, Entity e);
void AnimRender(world_t* w, Entity e);
void AnimSystem(world_t* w, Entity e);
void AnimRegister(world_t* w);
void AnimSink(void* obs_data, void* sub, payload_t*);

void InputLoad(world_t* w, Entity e);
void InputSystem(world_t* w, Entity e);
void InputRegister(world_t* w);

void OnForceEvent(event_t* ev, void* data);
void ForceLoad(world_t* w, Entity e);
void ForceSystem(world_t* w, Entity e);
void ForceCleanup(world_t* w, Entity e);
void ForceSink(void* obs_data, void* sub, payload_t*);

void PhysicsRegister(world_t* w);
void PhysicsLoad(world_t* w, Entity e);
void PhysicsSystem(world_t* w, Entity e);
void PhysicsCollision(world_t* w, Entity e);
void PhysicsDebug(world_t* w, Entity e);
void PhysicsPrep(world_t* w);

void LevelLoad(world_t* w, Entity e);
void LevelReady(world_t* w, Entity e);
void LevelSystem(world_t* w, Entity e);
void LevelRender(world_t* w, Entity e);

void RenderInit(world_t* w);
void RenderLoad(world_t* w, Entity e);
void RenderBegin(world_t* w, Entity e);
void RenderDraw(world_t* w, Entity e);
void RenderEnd(world_t* w, Entity e);

void CameraLoad(world_t* w, Entity e);
void CameraReady(world_t* w, Entity e);
void CameraTarget(world_t* w, Entity e);
void CameraSystem(world_t* w, Entity e);
void CameraBegin(world_t* w, Entity e);
void CameraEnd(world_t* w, Entity e);

void ObserveInit(world_t* w);
void ObserveReady(world_t* w, Entity e);
void SubjectLoad(world_t* w, Entity e);
void SubjectSystem(world_t* w, Entity e);

typedef struct{
  int     count, cap;
  int     *ents;
}sprite_layer_t;
extern sprite_layer_t SPRITE_LAYERS[LAYER_DONE];

void SpritesInit(world_t* w);
void SpriteLoad(world_t* w, Entity e);
void SpriteDrawPrep(world_t* w);

typedef struct {
  Entity  ents[MAX_PARTICLES];
  bool    in_use[MAX_PARTICLES];
} particle_layer_t;

void ParticleEmitterLoad(world_t* w, Entity e);
void ParticleEmitterSystem(world_t* w, Entity e);
void ParticleSystem(world_t* w, Entity e);
void ParticleCleanup(world_t* w, Entity e);
void ParticlesInit(world_t* w);

void BehaviorSystem(world_t* w, Entity e);
void BehaviorLoad(world_t* w);
void BehaviorRegister(world_t* w, Entity e);

void StateBegin(world_t* w, Entity e);
void StateRegister(world_t* w);

void CombatLoad(world_t* w, Entity e);
void CombatPrep(world_t* w);
void CombatSystem(world_t* w, Entity e);

void TeamLoad(world_t* w);
void TeamRegister(world_t* w, Entity e);

void ExpirationSystem(world_t* w, Entity e);

typedef struct{
  const char* name;
  void*       func;
}system_function_lookup_t;

static system_function_lookup_t FUNCTION_LOOKUP[] = {
  {"DebugSystem",          DebugSystem},
  {"PositionRegister",    PositionRegister},
  {"PositionLoad",        PositionLoad},
  {"PositionPrep",        PositionPrep},

  {"AnimLoad",            AnimLoad},
  {"AnimReady",           AnimReady},
  {"AnimRender",          AnimRender},
  {"AnimSystem",          AnimSystem},
  {"AnimRegister",        AnimRegister},

  {"InputLoad",           InputLoad},
  {"InputRegister",       InputRegister},
  {"InputSystem",         InputSystem},

  {"OnForceEvent",        OnForceEvent},
  {"ForceLoad",           ForceLoad},
  {"ForceSystem",         ForceSystem},
  {"ForceCleanup",        ForceCleanup},

  {"PhysicsRegister",     PhysicsRegister},
  {"PhysicsLoad",         PhysicsLoad},
  {"PhysicsSystem",       PhysicsSystem},
  {"PhysicsCollision",    PhysicsCollision},
  {"PhysicsDebug",        PhysicsDebug},
  {"PhysicsPrep",         PhysicsPrep},

  {"LevelLoad",           LevelLoad},
  {"LevelReady",          LevelReady},
  {"LevelSystem",         LevelSystem},

  {"RenderInit",          RenderInit},
  {"RenderLoad",          RenderLoad},
  {"RenderBegin",         RenderBegin},
  {"RenderDraw",          RenderDraw},
  {"RenderEnd",           RenderEnd},

  {"ObserveInit",         ObserveInit},
  {"ObserveReady",        ObserveReady},

  {"SubjectLoad",         SubjectLoad},
  {"SubjectSystem",       SubjectSystem},

  {"CameraSystem",        CameraSystem},
  {"CameraBegin",         CameraBegin},
  {"CameraEnd",           CameraEnd},
  {"CameraLoad",          CameraLoad},
  {"CameraReady",         CameraReady},
  {"CameraTarget",        CameraTarget},

  {"ParticleEmitterLoad",   ParticleEmitterLoad},
  {"ParticleEmitterSystem", ParticleEmitterSystem},
  {"ParticleSystem",        ParticleSystem},
  {"ParticleCleanup",       ParticleCleanup},
  {"ParticlesInit",         ParticlesInit},

  {"SpritesInit",         SpritesInit},
  {"SpriteLoad",          SpriteLoad},
  {"SpriteDrawPrep",      SpriteDrawPrep},

  {"BehaviorSystem",      BehaviorSystem},
  {"BehaviorLoad",        BehaviorLoad},
  {"BehaviorRegister",    BehaviorRegister},

  {"StateBegin",          StateBegin},
  {"StateRegister",        StateRegister},

  {"CombatPrep",          CombatPrep},

  {"TeamLoad",            TeamLoad},
  {"TeamRegister",        TeamRegister},

  {"ExpirationSystem",    ExpirationSystem}
};

#endif
