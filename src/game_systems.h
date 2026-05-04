#ifndef __GAME_SYS__
#define __GAME_SYS__
#include "game_register.h"

void OnPositionEvent(event_t* ev, void* data);
void PositionLoad(world_t* w, Entity e);

void AnimLoad(world_t* w, Entity e);
void AnimSystem(world_t* w, Entity e);
void AnimRender(world_t* w, Entity e);

void OnInputEvent(event_t* ev, void* data);
void InputLoad(world_t* w, Entity e);
void InputSystem(world_t* w, Entity e);

void OnForceEvent(event_t* ev, void* data);
void PhysicsLoad(world_t* w, Entity e);
void PhysicsSystem(world_t* w, Entity e);
void PhysicsCollision(world_t* w, Entity e);

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

void BehaviorSystem(world_t* w, Entity e);

void CombatLoad(world_t* w, Entity e);
void CombatSystem(world_t* w, Entity e);

#endif
