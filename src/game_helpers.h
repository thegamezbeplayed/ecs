#ifndef __GAME_HELPERS__
#define __GAME_HELPERS__

#include "game_define.h"

static bool COMPARE_ALL_BOOL(bool *all, int count){
  for (int i = 0; i < count; i++)
    if(!all[i])
      return false;

  return true;
}

static collision_d* GetAnimCollData( anim_comp_t* ac, int index){
  for(int i = 0; i < MAX_SLICES; i++){
    if(ac->hurtboxes[i].frame == index)
      return &ac->hurtboxes[i];
  }

  return NULL;
}

#endif
