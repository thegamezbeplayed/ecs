#ifndef __GAME_SYS__
#define __GAME_SYS__
#include "game_register.h"

void OnPositionEvent(event_t* ev, void* data);
void OnInputEvent(event_t* ev, void* data);
void LoadAnims(world_t* w, Entity e);
void AnimSystem(world_t* w, Entity e);
void AnimRender(world_t* w, Entity e);
//behavior_tree_node_t* InitBehaviorTree(behavior_st*, BehaviorID id);

#endif
