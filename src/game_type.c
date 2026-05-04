#include "game_types.h"
#include "game_define.h"
#include "game_helpers.h"

position_t* InitPosition(Vector2 pos){
  position_t* p = GameCalloc("InitPosition", 1, sizeof(position_t));

  p->last_vpos = p->vpos = pos;

  p->vdest = p->dir = VEC_UNSET;

  return p;

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
