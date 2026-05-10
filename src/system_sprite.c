#include "game_assets.h"
#include "game_systems.h"

void SpriteRender(world_t* w, Entity e){
  sprite_t* s      = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  position_t* pos  = GET_COMPONENT(w, e, position_t, POS_ID);


/*
  sprite_t* spr = &s->sprite;
  sprite_slice_t* slice = &SHEETS[spr->sheet_id].sprites[spr->index].slice;
  if(!slice)
    return;

  DrawSlice(slice, pos->vpos, 0);
  */
}
