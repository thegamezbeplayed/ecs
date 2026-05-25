#include "system_define.h"
#include "game_types.h"

void PositionSink(void* obs_data, void* sub, payload_t* pl ){
  position_t* p = obs_data;
  rigid_body_t* b = pl->data;

  Vector2 pos = b->bounds.pos;

  PositionSet(p, pos);
}

void PositionPrep(world_t* w){
  component_pool_t* pos = w->pools[POS_ID];

  if(!pos->dirty || !pos->updated)
    return;

  SpatialHashGridClear(&w->grid);

  system_t* pt = HashGet(&w->sys_map, hash_str_64("Positions"));
  entity_iter_t* iter = SystemGetIter("Positions");
  EntityIterStart(w, iter, pt);

   while(EntityIterNext(iter, w)){
     Entity e = iter->current;
     position_t* p = ComponentGet(w, e, POS_ID);

     SpatialHashGridInsert(&w->grid, e.id, p->pos, 32);
   }
}

void PositionLoad(world_t* w, Entity e){
  position_t* p = ComponentGet(w, e, POS_ID);

  //TODO NEED ENTITY SIZE
  SpatialHashGridInsert(&w->grid, e.id, p->pos, 32);
}

void PositionRegister(world_t* w){
  LookAddSink("Position", PositionSink);
  //TODO GET GRID SIZE DEFINE
  int cap = (MAX_ENTITIES - (MAX_PARTICLES * LAYER_TOP))/4;
  w->grid = *SpatialHashGrid(96, cap);
}
