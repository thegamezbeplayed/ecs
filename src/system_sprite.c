#include "game_assets.h"
#include "game_systems.h"

void SpriteRender(world_t* w, Entity e){
  spr_comp_t*  s = GET_COMPONENT(w, e, spr_comp_t, SPR_ID);
  pos_comp_t*  p = GET_COMPONENT(w, e, pos_comp_t, POS_ID);

  position_t* pos = &p->pos;


  sprite_t* spr = &s->sprite;
  sprite_slice_t* slice = &SHEETS[spr->sheet_id].sprites[spr->index].slice;
  if(!slice)
    return;

  DrawSlice(slice, pos->vpos, 0);
}
