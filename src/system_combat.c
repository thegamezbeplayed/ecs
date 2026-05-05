#include "game_assets.h"
#include "game_systems.h"

void CombatLoad(world_t* w, Entity e){
  anim_comp_t* ac = GET_COMPONENT(w, e, anim_comp_t, ANIM_ID);
  phys_comp_t* pc = GET_COMPONENT(w, e, phys_comp_t, PHYS_ID);
  //stat_comp_t* sc = GET_COMPONENT(w, e, stat_comp_t, STAT_ID);

  Vector2 size = VEC_NEW(ac->hitbox.wid, ac->hitbox.hei);

  RigidBodySetBounds(&pc->rb, size, size);

}

void CombatSystem(world_t* w, Entity e){

}
