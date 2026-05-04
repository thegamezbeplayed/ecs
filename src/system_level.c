#include "game_register.h"
#include "game_systems.h"

static choice_pool_t* locations;

void LevelLoad(world_t* w, Entity e){
  lvl_comp_t* l = GET_COMPONENT(w, e, lvl_comp_t, LVL_ID);

  l->bounds = Rect(0, 0, l->wid, l->hei);
}

void LevelReady(world_t* w, Entity e){

}

void LevelSystem(world_t* w, Entity e){
}

void LevelRender(world_t* w, Entity e){
  lvl_comp_t* l = GET_COMPONENT(w, e, lvl_comp_t, LVL_ID);

  DrawRectangleLinesEx(l->bounds, 1.5f, RED);


}
