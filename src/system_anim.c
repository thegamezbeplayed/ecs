#include "game_assets.h"
#include "game_systems.h"
#include "game_helpers.h"

void AnimSet(anim_comp_t* ac, anim_t* a, AnimState s){

}

void AnimEvent(event_t* ev, void* data){
  anim_comp_t* ac = data;

  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];

  AnimState s = ANIM_NONE;
  switch(EVENT_ID(ev->type)){
    case COMB_EVENT_HIT:
      s = ANIM_HURT;
      break;
    default:
      return;
      break;
  }

  AnimSet(ac, a, s);
}

void AnimInputEvent(event_t* ev, void* data){
  anim_comp_t* ac = data;
  input_t*       in = ev->data;

  int state_dir = in->angle/90;

  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];

  AnimState s = ANIM_NONE;
  switch(EVENT_ID(ev->type)){
    case INPUT_EVENT_MOVE:
      s = ANIM_WALK;
      break;
    case INPUT_EVENT_KEY_RELEASE:
      s = ANIM_IDLE;
      break;
    case INPUT_EVENT_ATTACK:
      s = ANIM_ATTACK;
      break;
    default:
      TraceLog(LOG_WARNING, "==== ANIM INPUT UNKOWN EVENT ====\n %i", EVENT_ID(ev->type));
      break;

  }

  if(!AnimPlayerState(ap, a, s))
    return;

  ap->dir = state_dir;

  if(!a)
    return;

  //if(AnimSetState(a, ANIM_START))

}

void AnimBehaviorHandler(world_t* w, Entity e, anim_comp_t* ac, anim_t* a){
  notification n;
  AnimBehavior bev = ANIM_BLANK;
  switch(ac->event){
    case ANIM_EVENT_FRAME_END:
      break;
    case ANIM_EVENT_SEQ_END:
      bev = a->on_end;
      break;
    case ANIM_EVENT_FRAME_START:
      bev = a->on_frame_start[a->cur_index];
      break;
    default:
      return;
      break;
  }

  switch(bev){
    case ANIM_SUSPEND:
      ac->player.state = ANIM_IDLE;
      break;
    case ANIM_HURTBOX:
      if(!GameCheckInteraction(e.id, e.id, "ANIM_ATTACK"))
        return;

      int rate = a->duration;
      GameInteraction(e.id, e.id, "ANIM_ATTACK", rate);

      n = PhysEvent_ToNotif(PHYS_EVENT_SPAWN);
      ac->hurtboxes[a->hurtbox_index].duration = rate;
      GameEvent(n, &ac->hurtboxes[a->hurtbox_index], e.id);
      break;
  }
}

void AnimLoad(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);

  input_comp_t* in = GET_COMPONENT(w, e, input_comp_t, INPUT_ID);

  notification n = CombatEvent_ToNotif(COMB_EVENT_HIT);
  TargetSubscribe(n, AnimEvent, ac, e.id );

  if(!in)
    return;

  n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, AnimInputEvent, ac, e.id );

  n = InputEvent_ToNotif(INPUT_EVENT_ATTACK);
  TargetSubscribe(n, AnimInputEvent, ac, e.id );

  n = InputEvent_ToNotif(INPUT_EVENT_KEY_RELEASE);
  TargetSubscribe(n, AnimInputEvent, ac, e.id );
}

void AnimSystem(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  position_t* pos = &p->pos;

  int spr_index = a->frames[a->cur_index];

  AnimEventID ev = AnimPlay(a);

  ac->event = ev;
  AnimBehaviorHandler(w, e, ac, a);
}

void AnimRender(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  position_t* pos = &p->pos;
  anim_player_t* ap = &ac->player;

  anim_t* a = &ac->sequences[ap->state][ap->dir];

  if(!a)
    return;

  int spr_index = a->frames[a->cur_index];

  sprite_slice_t* spr = &SHEETS[ap->sheet_id].sprites[spr_index].slice;
  if(!spr)
    return;

  DrawSlice(spr, pos->vpos, 0);
}
