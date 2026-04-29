#include "game_physics.h"
#include "game_process.h"

force_t* ForceFromVec2(ForceType type, Vector2 vec){
  force_t* g = GameCalloc("ForceFromVec2", 1, sizeof(force_t));

  g->type = type; 
  g->accel = vec;
  g->speed = 0.125f;
  g->max_velocity = MAX_VELOCITY;
  g->friction = Vector2One();
  g->on_react = CollisionNoAction;
  return g;

}

force_t* ForceBump(Vector2 acc){
  force_t* f = GameCalloc("ForceBump", 1, sizeof(force_t));

  f->type = FORCE_IMPULSE;
  f->accel = acc;
  f->max_velocity = MAX_VELOCITY;
  f->friction = VEC_BOTH(0.295f);

  f->threshold = 0.067f;
  f->speed = 0.895f;
  f->on_react = ForceReactBump;

  return f;
}

void ForceSetAccel(rigid_body_t *b, force_t* f, Vector2 acc){
  f->accel = acc;
}

void ForceAddMagnitude(force_t* f, Vector2 mag){
  Vector2 accel = Vector2Scale(mag, f->speed);
  f->accel = Vector2Add(accel, f->vel);
}

void ForceSetDir(rigid_body_t *b, force_t* f, Vector2 dir){
  f->dir = dir;
  Vector2 accel = Vector2Scale(dir,f->speed);

  ForceSetAccel(b, f,accel);
}

void ForceKill(rigid_body_t *b, force_t* f){
  HashRemove(&b->apply, f->uid);
}

void ForceApply(rigid_body_t* b, force_t* f){
  if(HashFull(&b->apply))
    return;

  f->on_end = ForceKill;

  f->is_active = true;
  f->uid = MakeGUID("FORCE", f->type);
  HashPut(&b->apply, f->uid, f);
}

void ForceReactBump(rigid_body_t* a, rigid_body_t* b, force_t *f){

  Rectangle collider = {
          .x =      a->bounds.pos.x,
          .y =      a->bounds.pos.y,
          .width =  a->bounds.width,
          .height = a->bounds.height,
          };

  Rectangle target = {
          .x =      b->bounds.pos.x,
          .y =      b->bounds.pos.y,
          .width =  b->bounds.width,
          .height = b->bounds.height,
          };

  Rectangle overlap;//TODO use this maybe instead of pen
  Vector2 surface_normal = math_normal_rec(collider, target,&overlap);


  Vector2 angBetween = vec_dir_between(b->bounds.pos, a->bounds.pos);

  float penAmount = fmaxf(overlap.width,overlap.height);

  float overlapRatio = penAmount/(fmaxf(collider.width,collider.height)+fmaxf(target.width,target.height));

  Vector2 penetration = Vector2Scale(angBetween,penAmount);

  Vector2 bump = Vector2Scale(Vector2Reflect(a->vel,surface_normal), a->restitution*overlapRatio);
  //bump = Vector2Add(bump,Vector2Scale(b->vel,b->restitution));

 float speed = f->speed +  Vector2Length(Vector2Max(a->vel,b->vel));
 if(Vector2Length(bump) == 0 && Vector2Length(penetration)==0)
  return;

 Vector2 rForce = Vector2Add(bump,penetration);

 if(Vector2DotProduct(rForce,Vector2Rotate(angBetween,PI))> 0)
   return;

 force_t *reaction = ForceFromVec2(f->type,Vector2ClampValue(rForce,1,speed));

 reaction->threshold = f->threshold;
 reaction->friction = f->friction;
  // Apply impulse
  ForceApply(a,reaction);

}

bool ForceStep(force_t *force, bool accelerate){
  if(accelerate && Vector2Length(force->accel) > 0){
    force->vel = Vector2Add(force->vel,force->accel);
    force->vel = Vector2Lerp(force->vel, Vector2Zero(), 1-force->friction.x);
  }
  else{
    force->vel = Vector2Multiply(force->vel,force->friction);

    if(Vector2Length(force->vel) < force->threshold)
      force->vel = Vector2Zero();
  }

  force->accel = Vector2Zero();

  force->vel = Vector2ClampValue(force->vel,0,force->max_velocity);

  return Vector2Length(force->vel) > force->threshold;
}

