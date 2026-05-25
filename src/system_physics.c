#include "system_define.h"
#include "process_event.h"
#include "tool_lookup.h"

void ForceSink(void* obs_data, void* sub, payload_t* pl){
  force_t* f = obs_data;
  if(pl->type_id == INPUT_ID){
      input_t* in = pl->data;
      action_key_t* a = InputGetAction(in->last_key);
      ForceSetDir(f, a->dir);
      return;
  }
  if(pl->type_id == POS_ID){
    position_t* p = pl->data;
    ForceSetDir(f, p->dir_step);
    return;
  }
}

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
          rigid_body_t* rb= GET_COMPONENT(&world, e, rigid_body_t, PHYS_ID);
          rigid_body_t* other = GET_COMPONENT(&world, *tar, rigid_body_t, PHYS_ID);
          if(other->is_static)
            return;

          force_t* react = ForceReactBump(rb, other, f);
          if(!react)
            return;

          Entity fent = EntityCreate(&world.manager);

          react->is_active = true;
          react->kill_on_end = true;
          force_t* f = ComponentAdd(&world, fent, FORCE_ID);
          memcpy(f, react , sizeof(force_t));
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

      Entity b = EntityCreate(&world.manager);

      rigid_body_t* rb = ComponentAdd(&world, b, PHYS_ID);
      position_t* p = ComponentAdd(&world, b, POS_ID);

      Vector2 pos = Vector2Inc(body->bounds.pos, coll->x, coll->y);

      memcpy(rb, InitRigidBody(pos, coll->shape, coll->wid, coll->hei),
          sizeof(rigid_body_t));

      rb->is_static = true;

      memcpy(p, InitPosition(pos), sizeof(position_t));

      rb->on_coll = PHYS_EVENT_HIT;
      EntityAddRelation(&world, b, REL_ChildOf, e);
      notification n = PhysEvent_ToNotif(PHYS_EVENT_DESTROY);

      lifetime_t* lf = ComponentAdd(&world, b, EXPIR_ID);

      LifetimeSet(lf, coll->duration);
      break;
    case COMB_EVENT_HIT:
      
      break;
  }
}

void PhysicsRegister(world_t* w){
  LookAddSink("Force", ForceSink);
  system_t* pt = HashGet(&w->sys_map, hash_str_64("Physics"));
  entity_iter_t* iter = SystemGetIter("Physics");

  EntityIterStart(w, iter, pt);
}

void PhysicsPrep(world_t* w){
  component_pool_t* bodies = w->pools[PHYS_ID];

  if(!bodies->dirty)
    return;
  
  system_t* pt = HashGet(&w->sys_map, hash_str_64("Physics"));
  entity_iter_t* iter = SystemGetIter("Physics");
  EntityIterStart(w, iter, pt);

  bodies->dirty = false;
}

void PhysicsLoad(world_t* w, Entity e){
  rigid_body_t* rb = GET_COMPONENT(w, e, rigid_body_t, PHYS_ID);
  position_t*  p = GET_COMPONENT(w, e, position_t, POS_ID);

  notification n = PhysEvent_ToNotif(PHYS_EVENT_SPAWN);
  SubscribeEntity(n, OnPhysEvent, rb, e.id);

  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);

  if(!ac)
    return;

  Vector2 size = VEC_NEW(ac->hitbox.wid, ac->hitbox.hei);
  RigidBodySetBounds(rb, size);

  Vector2 offset = VEC_NEW(ac->hitbox.x, ac->hitbox.y);
  RigidBodySetOffset(rb, offset);
}

void PhysicsCollision(world_t* w, Entity e){
  rigid_body_t* body = GET_COMPONENT(w, e, rigid_body_t, PHYS_ID);
  rigid_body_t* tar = NULL;
  entity_iter_t* iter = SystemGetIter("Physics");
  while(EntityIterNext(iter, w)){

    Entity other = iter->current;

    if(EntityGetRelationTarget(w, e, REL_ChildOf).id == other.id)
      continue;

    if(EntityGetRelationTarget(w, other, REL_ChildOf).id == e.id)
      continue;

    if(e.id == other.id)
      continue;

    notification n = PhysEvent_ToNotif(body->on_coll);
    if(!GameCheckInteraction(e.id, other.id, n))
      continue;

    tar = GET_COMPONENT(w, other, rigid_body_t, PHYS_ID);

    if(!tar)
      continue;

    if(!CheckCollision(body, tar, 0))
      continue;

    GameEvent(n, &other, e.id);

    int rate = imax(body->col_rate, tar->col_rate);
    
    GameInteraction(e.id, other.id, n, rate);
    return;
  }

  EntityIterReset(iter);
}

void PhysicsSystem(world_t* w, Entity e){
  rigid_body_t* b = GET_COMPONENT(w, e, rigid_body_t, PHYS_ID);

  if(Vector2Length(b->vel) > 0){
    b->bounds.pos = Vector2Add(b->bounds.pos, b->vel);

    notification n = PosEvent_ToNotif(POS_EVENT_STEP);
    GameEvent(n, b, e.id);
    ComponentUpdate(w, e, PHYS_ID);
  }
  
  b->vel = VECTOR2_ZERO;
}

void PhysicsDebug(world_t* w, Entity e){
 rigid_body_t* b = GET_COMPONENT(w, e, rigid_body_t, PHYS_ID);

  switch(b->bounds.shape){
    case SHAPE_CIRCLE:
      DrawCircleLinesV(b->bounds.pos, b->bounds.radius, BLUE);
      break;
    case SHAPE_REC:
      Rectangle rec = RigidBodyGetBoundsRec(b);
      DrawRectangleLinesEx(rec, 1.5, BLUE);

      break;
  }
}

void ForceLoad(world_t* w, Entity e){
  force_t* f = GET_COMPONENT(w, e, force_t, FORCE_ID);
  
  if(f->event == PHYS_EVENT_NONE)
    return;

  if(!EntityHasRelation(w, e, REL_AppliesTo))
     return;

  Entity rel = EntityGetRelationTarget(w, e, REL_AppliesTo);

  notification n = PhysEvent_ToNotif(f->event);
  SubscribeEntity(n, OnForceEvent, f, rel.id);

}

void ForceSystem(world_t* w, Entity e){
   force_t* f = GET_COMPONENT(w, e, force_t, FORCE_ID);

   if(!f->is_active)
     return;

   Entity be = e;
   rigid_body_t* rb = GET_COMPONENT(w, e, rigid_body_t, PHYS_ID);
   if(!rb && !EntityHasRelation(w, e, REL_AppliesTo))
     return;
   else if(!rb){
     be = EntityGetRelationTarget(w, e, REL_AppliesTo);
     rb = GET_COMPONENT(w, be, rigid_body_t, PHYS_ID);
   }

   if(!rb)
     return;

   f->is_active = ForceStep(f, f->is_active);
   ForceApply(rb, f);

   ComponentUpdate(w, be, PHYS_ID);

}

void ForceCleanup(world_t* w, Entity e){
  force_t* f = GET_COMPONENT(w, e, force_t, FORCE_ID);

  if(f->is_active)
    return;

  if(!f->kill_on_end)
    return;

  Entity rel = EntityGetRelationTarget(w, e, REL_AppliesTo);

  notification n = PhysEvent_ToNotif(PHYS_EVENT_FORCE_END);
  GameEvent(n, f, rel.id);

  EntityRelationEnd(w, e);

}
