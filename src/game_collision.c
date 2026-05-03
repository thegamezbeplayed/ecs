#include "game_physics.h"
#include "game_process.h"
#include "game_define.h"
#include "game_helpers.h"

rigid_body_t* InitRigidBody(Vector2 pos, ShapeType shape, float wid, float hei){
  rigid_body_t* b = GameCalloc("InitRigidBody", 1, sizeof(rigid_body_t));

  b->bounds.shape = shape;
  b->bounds.pos = pos;
  switch(shape){
    case SHAPE_CIRCLE:
      b->bounds.radius = wid;
      b->bounds.height = wid *2;
      b->bounds.width = wid *2;
      break;
    case SHAPE_REC:
      b->bounds.width = wid;
      b->bounds.height = hei;
      break;
  }

  b->col_rate = 6;
  return b;
}

void RigidBodyGiveForce(rigid_body_t* b, force_t* f){
  b->has[f->type] = *f;
}

void RigidBodySteer(rigid_body_t* b, Vector2 dir){
  force_t* f = &b->has[FORCE_STEERING];

  if(!f->is_active){
    f->on_end = ForceEnd;
    f->is_active = true;
  }
  
  if(!vec_compare(dir, f->dir))
    ForceSetDir(b, f, dir);
  else
    ForceAddMagnitude(f, dir);
}

bool CheckCollision(rigid_body_t *a, rigid_body_t *b, int len) {
  bool col = false;
  switch(COMBO_KEY(a->bounds.shape, b->bounds.shape)){
    case COMBO_KEY(SHAPE_CIRCLE, SHAPE_CIRCLE):
      col = CheckCollisionCircles(a->bounds.pos,a->bounds.radius, b->bounds.pos, b->bounds.radius+len);
      break;
    case COMBO_KEY(SHAPE_REC, SHAPE_CIRCLE):
      col = CheckCollisionCircleRec(b->bounds.pos, b->bounds.radius, (Rectangle){
          .x =      a->bounds.pos.x,
          .y =      a->bounds.pos.y,
          .width =  a->bounds.width,
          .height = a->bounds.height
          });
      break;
    case COMBO_KEY(SHAPE_CIRCLE,SHAPE_REC):
      col = CheckCollisionCircleRec(a->bounds.pos, a->bounds.radius, (Rectangle){
          .x =      b->bounds.pos.x,
          .y =      b->bounds.pos.y,
          .width =  b->bounds.width,
          .height = b->bounds.height
          });
      break;
    case COMBO_KEY(SHAPE_REC,SHAPE_REC):
      Rectangle recA = RecFromCoords(a->bounds.pos.x,a->bounds.pos.y,a->bounds.width,a->bounds.height);

      Rectangle recB = RecFromCoords(b->bounds.pos.x,b->bounds.pos.y,b->bounds.width,b->bounds.height);

      col = CheckCollisionRecs(recA,recB);
      break;
  }

  return col;
}

bool RigidBodyCollide(rigid_body_t* a, rigid_body_t* b){
  if(b->is_static)
    return false;

  bool out = false;
  for(int i = 0; i < FORCE_NONE; i++){
    if(!a->has[i].on_react)
      continue;

    out = true;
    a->has[i].on_react(a, b, &a->has[i]);

  }

  return out;
}

int CollisionStep(rigid_body_t* a, rigid_body_t* b){
  if(!CheckCollision(a, b, 0))
    return -1;

  bool ares = RigidBodyCollide(a, b);
  bool bres = RigidBodyCollide(b, a);
  
  if(!ares && !bres)
    return -1;

  return imax(a->col_rate, b->col_rate);
}
