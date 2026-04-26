#include "game_types.h"

position_t* InitPosition(Cell pos){
  position_t* p = GameCalloc("InitPosition", 1, sizeof(position_t));

  p->pos = pos;
  p->last_pos = p->facing = CELL_UNSET;

  p->vpos = cell_to_vec(pos, CELL_WIDTH);

  p->last_vpos = p->dir = VEC_UNSET;

  return p;

}

void PositionAddStep(position_t* p, Vector2 v){
  p->last_vpos = p->vpos;
  p->vpos = Vector2Add(p->vpos,v);

}
