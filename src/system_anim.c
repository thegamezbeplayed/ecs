#include "game_assets.h"
#include "game_systems.h"
#include "game_helpers.h"

void AnimInputEvent(event_t* ev, void* data){
  anim_player_t* ap = data;
  input_t*       in = ev->data;

  int state_dir = in->angle/90;

  ap->dir = state_dir;

  switch(EVENT_ID(ev->type)){
    case INPUT_EVENT_MOVE:
      AnimPlayerState(ap, ANIM_WALK);
      break;
    case INPUT_EVENT_KEY_RELEASE:
      AnimPlayerState(ap, ANIM_IDLE);
      break;
    case INPUT_EVENT_ATTACK:
      AnimPlayerState(ap, ANIM_ATTACK);
      break;
    default:
      TraceLog(LOG_WARNING, "==== ANIM INPUT UNKOWN EVENT ====\n %i", EVENT_ID(ev->type));
      break;

  }
}

void AnimBehaviorHandler(world_t* w, Entity e, anim_comp_t* ac, anim_t* a){
  switch(a->on_end){
    case ANIM_SUSPEND:
      ac->player.state = ANIM_IDLE;
      break;
  }
}

void AnimLoad(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);

  input_comp_t* in = GET_COMPONENT(w, e, input_comp_t, INPUT_ID);

  if(!in)
    return;

  notification n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, AnimInputEvent, &ac->player, e.id );

  n = InputEvent_ToNotif(INPUT_EVENT_KEY_RELEASE);
  TargetSubscribe(n, AnimInputEvent, &ac->player, ACT_MOVE );
}

void AnimSystem(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  position_t* pos = &p->pos;

  int spr_index = a->frames[a->cur_index];

  if(!AnimPlay(a))
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
