#include "process_event.h"
#include "system_define.h"
#include "process_event.h"
#include "tool_lookup.h"
#include "system_events.h"

void AnimSink(void* obs_data, void* sub, payload_t* pl){
  anim_comp_t* ac = obs_data;

  switch(EVENT_ID(pl->event)){
    case POS_EVENT_STEP:
      if(pl->type_id != POS_ID){
        TraceLog(LOG_WARNING, "=== ANIM SINK BAD DATA\n Expected Pos ID %i but payload is type %i",
            POS_ID, pl->type_id);
        return;
      }
      AnimHandlePos(ac, pl->data);
      break;
    case BEHAVIOR_EVENT_STATE:
      if(pl->type_id != STATE_ID){
        TraceLog(LOG_WARNING, "=== ANIM SINK BAD DATA\n Expected Behavior ID %i but payload is type %i",
            STATE_ID, pl->type_id);
        return;
      }
      AnimHandleState(ac, pl->data);
      break;
    case INPUT_EVENT_MOVE:
      if(pl->type_id != INPUT_ID){
        TraceLog(LOG_WARNING, "=== ANIM SINK BAD DATA\n Expected Input ID %i but payload is type %i",
            INPUT_ID, pl->type_id);
        return;
      }
      AnimHandleInput(ac, pl->data);
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

  DrawSprite(spr, pos->pos);

}

void AnimReady(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  Entity rel = EntityGetRelationTarget(w, e, REL_Target);
  
  if(EntityValid(&w->manager, rel)){
    notification n = ParticleEvent_ToNotif(PARTICLE_EVENT_START);
    particle_emitter_t* ec = GET_COMPONENT(w, rel, particle_emitter_t, EMITTER_ID);
    if(ec){
      GameEvent(n, ec, rel.id);
    }
  }
}

void AnimLoad(world_t* w, Entity e){
}

void AnimSystem(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  sprite_t* spr = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  anim_player_t* ap = &ac->player;
  anim_t* a = &ac->sequences[ap->state][ap->dir];

  int spr_index = a->frames[a->cur_index];

  spr->index = spr_index;

  AnimPhase p = AnimPlay(a);

  if(p > ANIM_DONE)
    return;

  AnimEventID ev = a->on_phase[p];
  AnimHandleEvent(w, e, ac, ev);
}

void AnimRegister(world_t* w){
  LookAddSink("Animation", AnimSink);
}
