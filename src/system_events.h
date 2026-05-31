#ifndef __SYS_EVENT__
#define __SYS_EVENT__
#include "component_define.h"

void CombatHandleEvent(notification n, Entity *tar, Entity agg);
void CombatHandleBehavior(Entity e);
void StateHandleAnim(state_t* st, anim_comp_t* ac);

void AnimHandleInput(anim_comp_t*, input_t*);
void AnimHandleState(anim_comp_t*, state_t* st);
void AnimHandlePos(anim_comp_t* , position_t* p);
void AnimHandleEvent(world_t*, Entity e, anim_comp_t* , AnimEventID event);
void PhysHandleEvent(world_t*, Entity e, Entity tar , PhysicsEventID event);
#endif
