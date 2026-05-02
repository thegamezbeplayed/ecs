#include "game_assets.h"
#include "game_systems.h"

void AnimInputEvent(event_t* ev, void* data){
  anim_player_t* ap = data;
  input_t*       in = ev->data;

  int state_dir = in->angle/90;

  ap->dir = state_dir;

  switch(EVENT_ID(ev->type)){
    case INPUT_EVENT_MOVE:
      ap->state = ANIM_WALK;
      break;
    default:
      TraceLog(LOG_WARNING, "==== ANIM INPUT UNKOWN EVENT ====\n %i", EVENT_ID(ev->type));
      break;

  }
}


void AnimLoad(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);

  notification n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, AnimInputEvent, &ac->player, e.id );
}

void AnimSystem(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];

  if(!AnimPlay(a) && a->on_end)
    a->on_end(ap, a);
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
