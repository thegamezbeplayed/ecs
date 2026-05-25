#ifndef __SYS_EVENT__
#define __SYS_EVENT__
#include "game_control.h"
#include "anim_define.h"
#include "behavior_define.h"


void StateHandleAnim(state_t* st, anim_comp_t* ac);

void AnimHandleInput(anim_comp_t*, input_t*);
void AnimHandleState(anim_comp_t*, state_t* st);
void AnimHandlePos(anim_comp_t* , position_t* p);
void AnimHandleEvent(world_t*, Entity e, anim_comp_t* , AnimEventID event);
#endif
