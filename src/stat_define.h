#ifndef __GAME_STAT__
#define __GAME_STAT__
#include "game_strings.h"

typedef struct stat_s stat_t;

typedef void (*StatCallback)(struct stat_s* self, int old, int cur);
typedef float (*StatGetter)(struct stat_s* self);


struct stat_s{
  char          name[MAX_NAME_LEN];
  uint64_t      id;
  int         min;
  int         max;
  int         current;
  int         increment;
  StatCallback  on_change, on_full, on_empty;
};

#endif
