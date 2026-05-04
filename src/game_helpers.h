#ifndef __GAME_HELPERS__
#define __GAME_HELPERS__

//#include "scene_data.h"

static bool COMPARE_ALL_BOOL(bool *all, int count){
  for (int i = 0; i < count; i++)
    if(!all[i])
      return false;

  return true;
}

#endif
