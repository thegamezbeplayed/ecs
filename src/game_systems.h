#ifndef __GAME_SYS__
#define __GAME_SYS__
#include "game_define.h"
#include "component_define.h"

#define NUM_FUNCTIONS 32
system_t* SystemCreate(world_t* w, system_define_t* def);

void OnPositionEvent(event_t* ev, void* data);
void PositionLoad(world_t* w, Entity e);

void AnimLoad(world_t* w, Entity e);
//void AnimBehavior(world_t* w, Entity e);
void AnimSystem(world_t* w, Entity e);
void AnimRender(world_t* w, Entity e);

void OnInputEvent(event_t* ev, void* data);
void InputLoad(world_t* w, Entity e);
void InputSystem(world_t* w, Entity e);

void OnForceEvent(event_t* ev, void* data);
void ForceLoad(world_t* w, Entity e);
void ForceSystem(world_t* w, Entity e);

void PhysicsInit(world_t* w);
void PhysicsLoad(world_t* w, Entity e);
void PhysicsSystem(world_t* w, Entity e);
void PhysicsCollision(world_t* w, Entity e);
void PhysicsDebug(world_t* w, Entity e);

void LevelLoad(world_t* w, Entity e);
void LevelReady(world_t* w, Entity e);
void LevelSystem(world_t* w, Entity e);
void LevelRender(world_t* w, Entity e);

void RenderLoad(world_t* w, Entity e);
void RenderBegin(world_t* w, Entity e);
void RenderEnd(world_t* w, Entity e);

void CameraSystem(world_t* w, Entity e);

void CameraLoad(world_t* w, Entity e);
void CameraReady(world_t* w, Entity e);

void SpriteRender(world_t* w, Entity e);
void SpriteLoad(world_t* w, Entity e);

void BehaviorSystem(world_t* w, Entity e);

void CombatLoad(world_t* w, Entity e);
void CombatSystem(world_t* w, Entity e);

void ForceCleanup(world_t* w, Entity e);

void ExpirationSystem(world_t* w, Entity e);

typedef struct{
  const char* name;
  void*       func;
}system_function_lookup_t;

static system_function_lookup_t FUNCTION_LOOKUP[NUM_FUNCTIONS] = {
    {"OnPositionEvent",     OnPositionEvent},
    {"PositionLoad",        PositionLoad},

    {"AnimLoad",            AnimLoad},
    {"AnimSystem",          AnimSystem},
    {"AnimRender",          AnimRender},

    {"OnInputEvent",        OnInputEvent},
    {"InputLoad",           InputLoad},
    {"InputSystem",         InputSystem},

    {"OnForceEvent",        OnForceEvent},
    {"ForceLoad",           ForceLoad},
    {"ForceSystem",         ForceSystem},
    {"ForceCleanup",        ForceCleanup},

    {"PhysicsInit",         PhysicsInit},
    {"PhysicsLoad",         PhysicsLoad},
    {"PhysicsSystem",       PhysicsSystem},
    {"PhysicsCollision",    PhysicsCollision},
    {"PhysicsDebug",        PhysicsDebug},

    {"LevelLoad",           LevelLoad},
    {"LevelReady",          LevelReady},
    {"LevelSystem",         LevelSystem},
    {"LevelRender",         LevelRender},

    {"RenderLoad",          RenderLoad},
    {"RenderBegin",         RenderBegin},
    {"RenderEnd",           RenderEnd},

    {"CameraSystem",        CameraSystem},
    {"CameraLoad",          CameraLoad},
    {"CameraReady",         CameraReady},

    {"SpriteRender",        SpriteRender},

    {"SpriteLoad",          SpriteLoad},

    {"CombatLoad",          CombatLoad},
    {"CombatSystem",        CombatSystem},

    {"ExpirationSystem",    ExpirationSystem}
};

#endif
