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

          if(other->is_static)
            return;

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

void OnPhysEvent(event_t* ev, void* data){
  rigid_body_t* body = data;
  Entity e = EntityGet(&world.manager, ev->eid);

  switch(EVENT_ID(ev->type)){
    case PHYS_EVENT_SPAWN:
      collision_d* coll = ev->data;

      Entity rb = EntityCreate(&world.manager);

      phys_comp_t* ph = ComponentAdd(&world, rb, PHYS_ID);
      pos_comp_t* pc = ComponentAdd(&world, rb, POS_ID);

      Vector2 pos = Vector2Inc(body->bounds.pos, coll->posx, coll->posy);

      ph->rb = *InitRigidBody(pos, coll->shape, coll->wid, coll->hei);

      ph->rb.is_static = true;

      pc->pos = *InitPosition(pos);

      ph->rb.on_coll = COLL_HIT;
      TraceLog(LOG_INFO, "==== PHYS_EVENT_SPAWN ===\n Spawn Rigid Body %d for Ent %d at %i", rb.id, e.id, WorldGetTime());
      EntityAddRelation(&world, rb, REL_ChildOf, e);
      notification n = PhysEvent_ToNotif(PHYS_EVENT_DESTROY);
      ScheduleEvent(n, &rb, rb.id, TF_UPDATE, coll->duration);
      break;
    case PHYS_EVENT_DESTROY:
      world_t* w = data;
      Entity tar = EntityGet(&w->manager, ev->eid);
      EntityRelationEnd(w, tar);
      break;
    case COMB_EVENT_HIT:
      
      break;
  }
}

void PhysicsInit(world_t* w){
  notification n = PhysEvent_ToNotif(PHYS_EVENT_DESTROY);
  Subscribe(n, OnPhysEvent, w);
}

void PhysicsLoad(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  notification n = PosEvent_ToNotif(POS_EVENT_STEP);
  SubscribeEntity(n, OnPositionEvent, &p->pos, e.id);
  n = PhysEvent_ToNotif(PHYS_EVENT_SPAWN);
  SubscribeEntity(n, OnPhysEvent, &pc->rb, e.id);
  n = CombatEvent_ToNotif(COMB_EVENT_HIT);
  SubscribeEntity(n, OnPhysEvent, &pc->rb, e.id);

  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);

  if(!ac)
    return;

  Vector2 size = VEC_NEW(ac->hitbox.wid, ac->hitbox.hei);
  RigidBodySetBounds(&pc->rb, size);
}

void PhysicsCollision(world_t* w, Entity e){
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);
  phys_comp_t* opc = NULL;


  rigid_body_t* body = &pc->rb;
  while(EntityIterNext(w->iter, w)){

    Entity other = w->iter->current;

    if(EntityGetRelationTarget(w, other, REL_ChildOf).id == e.id)
      continue;

    if(e.id == other.id)
      continue;

    if(!GameCheckInteraction(e.id, other.id, "RB_COLL"))
      continue;


    opc = GET_COMPONENT(w, other, phys_comp_t, PHYS_ID);

    if(!opc)
      continue;

    rigid_body_t* tar = &opc->rb;
    
    if(!CheckCollision(body, tar, 0))
      continue;

    TraceLog(LOG_INFO, "== INTERACTION ==\n %d with %d at %i", e.id, other.id, WorldGetTime());

    notification n = PhysEvent_ToNotif(PHYS_EVENT_COLL);
    uint32_t evid = e.id;
    switch(body->on_coll){
      case COLL_FORCE:
        n = PhysEvent_ToNotif(PHYS_EVENT_COLL);
        break;
      case COLL_HIT:
        n = CombatEvent_ToNotif(COMB_EVENT_HIT);
        evid = other.id;
        break;
    }

    GameEvent(n, &other, evid);

    int rate = imax(body->col_rate, tar->col_rate);
    
    GameInteraction(e.id, other.id, "RB_COLL", rate);
    return;
  }

  EntityIterReset(w->iter);
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
      Rectangle rec = RECT(b->bounds.pos.x, b->bounds.pos.y, b->bounds.width, b->bounds.height);
      DrawRectangleLinesEx(rec, 1.5, BLUE);

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

   fc->f.is_active = ForceStep(&fc->f, fc->f.is_active);
   ForceApply(&pc->rb, &fc->f);

}

void ForceCleanup(world_t* w, Entity e){
  force_comp_t* fc = GET_COMPONENT(w, e, force_comp_t, FORCE_ID);

  if(fc->f.is_active)
    return;

  if(!fc->f.kill_on_end)
    return;

  Entity rel = EntityGetRelationTarget(w, e, REL_AppliesTo);

  notification n = PhysEvent_ToNotif(PHYS_EVENT_FORCE_END);
  GameEvent(n, &fc->f, rel.id);
  TraceLog(LOG_INFO, "== END RELATION ==\n %d on %d", e.id, rel.id);

  EntityRelationEnd(w, e);

}
