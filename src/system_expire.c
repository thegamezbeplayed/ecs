#include "system_define.h"

void ExpirationSystem(world_t* w, Entity e){
  lifetime_t* lf = GET_COMPONENT(w, e, lifetime_t, EXPIR_ID);
  if(!lf)
    return;

  if(WorldGetTime() < lf->expiration)
    return;

  EntityRelationEnd(w, e);
}
