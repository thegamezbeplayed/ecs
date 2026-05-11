#include "game_assets.h"
#include "game_systems.h"

void SpriteLoad(world_t* w, Entity e){

}

void SpriteRender(world_t* w, Entity e){
<<<<<<< HEAD
  sprite_t* spr      = GET_COMPONENT(w, e, sprite_t, SPR_ID);
=======
  sprite_t* s      = GET_COMPONENT(w, e, sprite_t, SPR_ID);
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
  position_t* pos  = GET_COMPONENT(w, e, position_t, POS_ID);


  sprite_slice_t* slice = &SHEETS[spr->sheet_id].sprites[spr->index].slice;
  if(!slice)
    return;

  DrawSlice(slice, pos->vpos, 0);
}
