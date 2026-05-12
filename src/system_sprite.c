#include "game_assets.h"
#include "game_systems.h"

void SpriteLoad(world_t* w, Entity e){

}

void SpriteRender(world_t* w, Entity e){
  sprite_t* s      = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  position_t* pos  = GET_COMPONENT(w, e, position_t, POS_ID);


  sprite_slice_t* slice = &SHEETS[s->sheet_id].sprites[s->index].slice;
  if(!slice)
    return;

  DrawSlice(slice, pos->vpos, 0);
}
