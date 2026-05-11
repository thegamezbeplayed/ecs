#include "game_define.h"
#include "component_define.h"
#include "game_helpers.h"
#include "game_process.h"

bool PositionInit(void* comp, component_entry_t* j){
  position_t* p = comp;

  return ParsePositionComponent(j->data, p);
}

position_t* InitPosition(Vector2 pos){
  position_t* p = GameCalloc("InitPosition", 1, sizeof(position_t));

  p->last_vpos = p->vpos = pos;

  p->vdest = p->dir = VEC_UNSET;

  return p;

}

void PositionSet(position_t* p, Vector2 pos){
  p->last_vpos = p->vpos;
  p->vpos = pos;
}

void PositionAddStep(position_t* p, Vector2 v){
  p->last_vpos = p->vpos;
  p->vpos = Vector2Add(p->vpos,v);

}

void PositionSetDest(position_t* p, Vector2 v){
  p->vdest = v;

  Vector2 dir = vec_dir_between(p->vpos, p->vdest);
  p->angle = angle_snap_to_card(dir);
}

bool CoordInit(void* comp, component_entry_t* data){

}

void PositionFromCoords(position_t* p, coordinate_t* coords, float scale){
  p->vpos = p->last_vpos = cell_to_vec(coords->pos, scale);

}
