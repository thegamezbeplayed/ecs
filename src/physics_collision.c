#include "game_helpers.h"
#include "component_define.h"

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

  b->col_rate = 15;
  return b;
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
      Rectangle recA = RigidBodyGetBoundsRec(a);

      Rectangle recB = RigidBodyGetBoundsRec(b);

      col = CheckCollisionRecs(recA,recB);
      break;
  }

  return col;
}

bool RigidBodyInit(void* comp, component_entry_t* j){
  rigid_body_t* rb = comp;

  return ParseRigidBodyComponent(j->data, rb);
}

