#include "game_utils.h"
#include "game_control.h"
#include "game_assets.h"
#include "game_register.h"

void AnimInputEvent(event_t* ev, void* data){
  input_t* in = ev->data;
  anim_player_t* ap = data;

  uint64_t group = hash_str_64(in->move);
  anim_t* a = AnimGet(ap, group);

  AnimSetSequence(ap, a);
}


void LoadAnims(world_t* w, Entity e){
  anim_player_t* ap = GET_COMPONENT(w, e, anim_player_t, ANIM_ID);
  TargetSubscribe("INPUT_MOVE", AnimInputEvent, ap, e.id );


  for (int j = 0; j < ap->num_seq; j++){
    anim_t* a = &ap->anims[j];

    //RegisterAnimation(st, a->group);
  }
}


