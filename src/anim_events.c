#include "system_events.h"

void AnimHandleEvent(world_t* w, Entity e, anim_comp_t* a, AnimEventID event){
  switch(event){
    case ANIM_EVENT_NONE:
      return;
      break;
    case ANIM_EVENT_SUSPEND:
      a->player.state = ANIM_IDLE;
      break;
  }

  ComponentUpdate(w, e, ANIM_ID);
}

void AnimHandleInput(anim_comp_t *ac, input_t* in){
  action_key_t* ak = InputGetAction(in->last_key);
  int dir = -1;
  AnimState s = ANIM_NONE;
  switch(ak->type){
    case ACT_MOVE:
      s = ANIM_WALK;
      dir = abs(vec_to_deg(ak->dir)/90);
      break;
    case ACT_ATTACK:
      break;
    case ACT_STOP:
      s = ANIM_IDLE;
      break;
    default:
      return;
      break;
  }

  AnimPlayerState(ac, s, dir);

}

void AnimHandleState(anim_comp_t *a, state_t* st){
  AnimState s = ANIM_NONE;

  switch(st->state){
    case STATE_ATTACK:
      s = ANIM_ATTACK;
      break;
    case STATE_DIE:
      s = ANIM_DIE;
      break;
    default:
      return;
      break;
  }

  AnimPlayerState(a, s, -1);
}

void AnimHandlePos(anim_comp_t *ac, position_t* p){
  anim_player_t* player = &ac->player;
  int dir = abs(vec_to_deg(p->dir_step)/90);
  AnimPlayerState(ac, ANIM_WALK, dir);

}
