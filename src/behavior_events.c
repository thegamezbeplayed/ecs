#include "system_events.h"

void StateHandleAnim(state_t* st, anim_comp_t* ac){
  switch(COMBO_KEY(st->state, ac->player.state)){
    case COMBO_KEY(STATE_ATTACK, ANIM_IDLE):
      BehaviorSetState(st, STATE_AGGRO);
      break;
    default:
  }

  return;
}
