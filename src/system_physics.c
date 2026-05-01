#include "game_systems.h"
#include "game_physics.h"

void OnForceEvent(event_t* ev, void* data){
  position_t* p = ev->data;
  rigid_body_t* b = data;

  Vector2 step = p->dir_step;

  RigidBodySteer(b, step);

//  PositionAddStep(p, step);
}

void PhysicsLoad(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  notification n = PhysEvent_ToNotif(PHYS_EVENT_ACCEL);
  SubscribeEntity(n, OnForceEvent, &pc->rb, e.id);

  n = PosEvent_ToNotif(POS_EVENT_STEP);
  SubscribeEntity(n, OnPositionEvent, &p->pos, e.id);
}

void PhysicsCollision(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);
  phys_comp_t* opc = NULL;

  system_t* phsys = &w->systems[PHYS_SYS];

  //THIS MAY BOTTLENECK FIND WAY TO ITERATE NEXT WITH EXISTING POOL
  component_pool_t* others = ComponentQueryInner(w, phsys);

  rigid_body_t* body = &pc->rb;
  for(int i = 0; i < others->size; i++){

    Entity other = { others->entities[i], w->manager.generation[others->entities[i]] };

    if(e.id == other.id)
      continue;

    if(GameCheckInteraction(e.id, other.id, "RB_COLL"))
      continue;


    opc = GET_COMPONENT(w, other, phys_comp_t, PHYS_ID);

    if(!opc)
      continue;

    rigid_body_t* tar = &opc->rb;
    
    int rate= CollisionStep(body, tar);
    if(rate  < 1 )
      continue;


    Vector2 apos = body->bounds.pos;
    Vector2 bpos = tar->bounds.pos;

    interaction_t* intact = InitInteraction(e.id, other.id, "RB_COLL", rate);

    GameInteraction(intact);
  }
}

void PhysicsSystem(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);

  rigid_body_t* b = &pc->rb;

  b->vel = VECTOR2_ZERO;
  bool step = false;

  for(int i = 0; i < FORCE_NONE; i++){
    force_t* f = &b->has[i];
    if(!f->is_active)
      continue;
    
    if(!ForceStep(f, true)){
      if(f->on_end)
        f->on_end(b, f);

      continue;
    }

    b->vel = Vector2Add(b->vel, f->vel);
    step = true;
  }


  for(int i = 0; i < MAX_FORCES; i++){
    force_t* f = &b->apply[i];
    if(!f->is_active)
      continue;

    if(!ForceStep(f, true)){
      if(f->on_end)
        f->on_end(b, f);

      continue;
    }
    step = true;
    b->vel = Vector2Add(b->vel,f->vel);
  }

  if(step){
    b->bounds.pos = Vector2Add(b->bounds.pos, b->vel);

    notification n = PosEvent_ToNotif(POS_EVENT_STEP);
    GameEvent(n, b, e.id);
  }
}

