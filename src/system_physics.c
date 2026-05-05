#include "game_systems.h"
#include "game_physics.h"

void OnForceEvent(event_t* ev, void* data){
  force_t* f = data;
  switch(EVENT_ID(ev->type)){
    case PHYS_EVENT_ACCEL:
      position_t* p = ev->data;
      Vector2 step = p->dir_step;

      ForceSetDir(f, step);
      break;
    case PHYS_EVENT_COLL:
      switch(f->react){
        case REACT_BUMP:
          Entity *tar = ev->data;
          Entity e = EntityGet(&world.manager, ev->eid);
          phys_comp_t* pc = GET_COMPONENT(&world, e, phys_comp_t, PHYS_ID);
          phys_comp_t* opc = GET_COMPONENT(&world, *tar, phys_comp_t, PHYS_ID);

          rigid_body_t* other = &opc->rb;

          force_t* react = ForceReactBump(&pc->rb, other, f);
          if(!react)
            return;


          Entity fent = EntityCreate(&world.manager);

          react->is_active = true;
          react->kill_on_end = true;
          force_comp_t* efc = ComponentAdd(&world, fent, FORCE_ID);
          efc->f = *react;

          EntityAddRelation(&world, fent, REL_AppliesTo, *tar);
          break;
        case REACT_BLOCK:
          break;
      }
      break;
  }


}

void PhysicsLoad(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  notification n = PosEvent_ToNotif(POS_EVENT_STEP);
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

    interaction_t* check = GameCheckInteraction(e.id, other.id, "RB_COLL");
    if(check)
      continue;


    opc = GET_COMPONENT(w, other, phys_comp_t, PHYS_ID);

    if(!opc)
      continue;

    rigid_body_t* tar = &opc->rb;
    
    if(!CheckCollision(body, tar, 0))
      continue;


    notification n = PhysEvent_ToNotif(PHYS_EVENT_COLL);

    GameEvent(n, &other, e.id);

    int rate = imax(body->col_rate, tar->col_rate);
    
    interaction_t* intact = InitInteraction(e.id, other.id, "RB_COLL", rate);
    GameInteraction(intact);

    return;
  }
}

void PhysicsSystem(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);

  rigid_body_t* b = &pc->rb;


  if(Vector2Length(b->vel) > 0){
    b->bounds.pos = Vector2Add(b->bounds.pos, b->vel);

    notification n = PosEvent_ToNotif(POS_EVENT_STEP);
    GameEvent(n, b, e.id);
  }
  
  b->vel = VECTOR2_ZERO;
}

void PhysicsDebug(world_t* w, Entity e){
 phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);

  rigid_body_t* b = &pc->rb;

  switch(b->bounds.shape){
    case SHAPE_CIRCLE:
      DrawCircleLinesV(b->bounds.pos, b->bounds.radius, BLUE);
      break;
    case SHAPE_REC:

      break;
  }

}

void ForceLoad(world_t* w, Entity e){
  force_comp_t* fc = GET_COMPONENT(w, e, force_comp_t, FORCE_ID);
  
  if(fc->f.event == PHYS_EVENT_NONE)
    return;

  if(!EntityHasRelation(w, e, REL_AppliesTo))
     return;

  Entity rel = EntityGetRelationTarget(w, e, REL_AppliesTo);

  notification n = PhysEvent_ToNotif(fc->f.event);
  SubscribeEntity(n, OnForceEvent, &fc->f, rel.id);

}

void ForceSystem(world_t* w, Entity e){
   force_comp_t* fc = GET_COMPONENT(w, e, force_comp_t, FORCE_ID);

   if(!EntityHasRelation(w, e, REL_AppliesTo))
     return;

   Entity rel = EntityGetRelationTarget(w, e, REL_AppliesTo);

   phys_comp_t* pc = GET_COMPONENT(w, rel, phys_comp_t, PHYS_ID);

   if(ForceStep(&fc->f, fc->f.is_active))
     ForceApply(&pc->rb, &fc->f);

}
