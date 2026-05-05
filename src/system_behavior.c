#include "game_register.h"
#include "game_systems.h"

#include "game_behaviors.h"

void OnBehaviorEvent(event_t* ev, void* data){
  position_t* p = ev->data;
  rigid_body_t* rb = data;

  Vector2 dir = Vector2Normalize(p->vdest);
  RigidBodySteer(rb, dir);
}

void BehaviorSystem(world_t* w, Entity e){
  ai_comp_t *ac = GET_COMPONENT(w, e, ai_comp_t, AI_ID);
  state_comp_t *sc = GET_COMPONENT(w, e, state_comp_t, STATE_ID);
}


