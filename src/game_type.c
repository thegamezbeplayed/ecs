#include "game_define.h"
#include "component_define.h"
#include "game_helpers.h"

bool PositionInit(void* comp, component_entry_t* j){
  position_t* p = comp;

  return ParsePositionComponent(j->data, p);
}

position_t* InitPosition(Vector2 pos){
  position_t* p = GameCalloc("InitPosition", 1, sizeof(position_t));

  p->last_pos = p->pos = pos;

  p->dest = p->dir = VEC_UNSET;

  return p;

}

void PositionSet(position_t* p, Vector2 pos){
  p->last_pos = p->pos;
  p->pos = pos;
}

void PositionAddStep(position_t* p, Vector2 v){
  p->last_pos = p->pos;
  p->pos = Vector2Add(p->pos,v);

}

void PositionSetDest(position_t* p, Vector2 v){
  p->dest = v;

  Vector2 dir = vec_dir_between(p->pos, p->dest);
  p->angle = angle_snap_to_card(dir);
}

