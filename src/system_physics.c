#include "game_register.h"
#include "game_systems.h"

static int steps = 0;
void PhysicsCollision(system_pool_t* s, component_registry_t* c){
  phys_st* ps = &s->phys;
  rigid_body_c* rbc = &c->bodies;
  EntityManager *em = &c->manager;

  for(int i = 0; i < rbc->map.size; i++){
    rigid_body_t* body = &rbc->dense[i];
    Entity* ea = ComponentGetEntity(em, &rbc->map, i);

    for(int j = 0; j < rbc->map.size; j++){
      if(i == j)
        continue;

      Entity* eb = ComponentGetEntity(em, &rbc->map, j);
      if(GameCheckInteraction(ea->id, eb->id, "RB_COLL"))
        return;

      rigid_body_t* other = &rbc->dense[j];
      int rate= CollisionStep(body, other);
      if(rate  < 1 )
        continue;

      Vector2 apos = body->bounds.pos;
      Vector2 bpos = other->bounds.pos;

      interaction_t* intact = InitInteraction(ea->id, eb->id, "RB_COLL", rate);

      GameInteraction(intact);
    }
  } 
}

void PhysicsSystem(system_pool_t* s, component_registry_t* c){
  phys_st* col = &s->phys;
  rigid_body_c* rbc = &c->bodies;
  EntityManager *em = &c->manager;

  for(int i = 0; i < rbc->map.size; i++){
    rigid_body_t* b = &rbc->dense[i];
    Entity* e = ComponentGetEntity(em, &rbc->map, i);

    b->vel = VECTOR2_ZERO;
    bool step = false;

    force_t* steer = ForceHas(b, MakeFUID("FORCE", FORCE_STEERING));
    if(steer && steer->is_active && ForceStep(steer, true)){
      b->vel = Vector2Add(b->vel, steer->vel);
      step = true;
    }

    hash_iter_t iter;
    HashStart(&b->apply, &iter);

    hash_slot_t* s = NULL;

    while((s = HashNext(&iter))){
      force_t* f = s->value;

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
      GameEvent("VEL_STEP", b, e->id);
    }
   }

}

void LoadPhysics(system_pool_t* s, component_registry_t* c){
  rigid_body_c* rbc = &c->bodies;
  EntityManager *em = &c->manager;

  position_c* p = &c->pos;

  int count = 0;
  for(int i = 0; i < p->map.size; i++){
    position_t* pos = &p->dense[i];
    Entity* e = ComponentGetEntity(em, &p->map, i);

    int bid = ComponentByEntity(&rbc->map, i);
    rigid_body_t* b =  &rbc->dense[bid];
    SubscribeEntity("VEL_STEP", OnPositionEvent, pos, e->id);

    if(e->id == PLAYER){
      count++;
      SubscribeEntity("INPUT_MOVE", OnInputEvent, b, e->id);
    }
  }
}

void InitPhysicsSystem(phys_st* p){
  RegisterScheduleState(GAME_READY, LoadPhysics);
  RegisterScheduleStep(UPDATE_PRE, PhysicsCollision);
  RegisterScheduleStep(UPDATE_POST, PhysicsSystem);
}
