#include "game_assets.h"
#include "game_systems.h"

void AnimInputEvent(event_t* ev, void* data){
  input_t* in = ev->data;
  anim_player_t* ap = data;

  uint64_t group = hash_str_64(in->move);
  anim_t* a = AnimGet(ap, group);

  AnimSetSequence(ap, a);
}


void LoadAnims(world_t* w, Entity e){
  anim_player_t* ap = GET_COMPONENT(w, e, anim_player_t, ANIM_ID);

  notification n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, AnimInputEvent, ap, e.id );

/*
  for (int j = 0; j < ap->num_seq; j++){
    anim_t* a = &ap->anims[j];

    //RegisterAnimation(st, a->group);
  }
  */
}

void AnimSystem(world_t* w, Entity e){
  anim_component_t* a = GET_COMPONENT(w, e, anim_component_t, ANIM_ID);
  AnimPlay(a->player, 1.0f);
}

void AnimRender(world_t* w, Entity e){
  anim_component_t* a = GET_COMPONENT(w, e, anim_component_t, ANIM_ID);
  pos_component_t*  p = GET_COMPONENT(w, e, pos_component_t, POS_ID);

  position_t* pos = p->pos;
  anim_player_t* ap = a->player;

  sprite_slice_t* frame = AnimGetFrame(ap);
  DrawSlice(frame, pos->vpos, 0);

}
