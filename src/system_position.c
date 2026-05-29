#include "system_define.h"
#include "game_types.h"

void PositionSink(void* obs_data, void* sub, payload_t* pl ){
  position_t* p = obs_data;
  rigid_body_t* b = pl->data;

  Vector2 pos = b->bounds.pos;

  PositionSetNext(p, pos);
}

void PositionPrep(world_t* w){
  component_pool_t* pos = w->pools[POS_ID];

  if(!pos->dirty && !pos->updated)
    return;

  SpatialHashGridClear(&w->grid);

  system_t* pt = HashGet(&w->sys_map, hash_str_64("Positions"));
  entity_iter_t* iter = SystemGetIter("Positions");
  EntityIterStart(w, iter, pt);

   while(EntityIterNext(iter, w)){
     Entity e = iter->current;
     position_t* p = ComponentGet(w, e, POS_ID);

     p->grid_bucket = SpatialHashGridInsert(&w->grid, e.id, p->pos, 32);
   }
   
   pos->updated = false;
   pos->dirty = false;
}

void PositionSystem(world_t* w, Entity e){
  position_t* p = ComponentGet(w, e, POS_ID);

  if(!PositionApplyNext(p))
    return;

  ComponentUpdate(w, e , POS_ID, PosEvent_ToNotif(POS_EVENT_SYNC));
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
