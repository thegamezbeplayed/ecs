#include "process_event.h"
#include "system_define.h"
#include "process_event.h"
#include "tool_lookup.h"

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

void AnimSink(void* obs_data, void* sub, void* ev_data){
  anim_comp_t* ac = obs_data;
  input_t* in = ev_data;

  action_key_t* ak = InputGetAction(in->last_key);
 
  int dir = -1; 
  AnimState s = ANIM_NONE;
  switch(ak->type){
    case ACT_MOVE:
      s = ANIM_WALK;
      dir = angle_snap_to_card(ak->dir)/90;
      break;
    case ACT_ATTACK:
      break;
    default:
      return;
      break;
  }

  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];

  if(!AnimPlayerState(ap, a, s))
    return;
  
  ap->dir = dir;
}

void AnimInputEvent(event_t* ev, void* data){
/*
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
*/
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

void AnimRender(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  sprite_t* spr = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];
  position_t* pos = GET_COMPONENT(w, e, position_t, POS_ID);

  int spr_index = a->frames[a->cur_index];
  spr->index = spr_index;

  DrawSprite(spr, pos->vpos);

}

void AnimReady(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  Entity rel = EntityGetRelationTarget(w, e, REL_Target);
  
  if(rel.id != INVALID_ENTITY.id){
    notification n = ParticleEvent_ToNotif(PARTICLE_EVENT_START);
    particle_emitter_t* ec = GET_COMPONENT(w, rel, particle_emitter_t, EMITTER_ID);
    if(ec){
      GameEvent(n, ec, rel.id);
    }
  }
}

void AnimLoad(world_t* w, Entity e){

  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
/*
  input_t* in = GET_COMPONENT(w, e, input_t, INPUT_ID);

  if(!in)
    return;

  notification n = InputEvent_ToNotif(INPUT_EVENT_MOVE);
  TargetSubscribe(n, AnimInputEvent, ac, e.id );

  n = InputEvent_ToNotif(INPUT_EVENT_ATTACK);
  TargetSubscribe(n, AnimInputEvent, ac, e.id );

  n = InputEvent_ToNotif(INPUT_EVENT_KEY_RELEASE);
  TargetSubscribe(n, AnimInputEvent, ac, e.id );
*/
}

void AnimSystem(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  sprite_t* spr = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];

  int spr_index = a->frames[a->cur_index];

  spr->index = spr_index;

  AnimEventID ev = AnimPlay(a);

  ac->event = ev;
  AnimBehaviorHandler(w, e, ac, a);
}

void AnimRegister(world_t* w){
  LookAddSink("Animation", AnimSink);
}
