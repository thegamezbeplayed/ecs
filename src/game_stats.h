#ifndef __GAME_STAT__
#define __GAME_STAT__
#include "game_strings.h"

typedef enum{
  STAT_NONE,
  STAT_HP,
  STAT_DMG,
  STAT_DONE
}StatType;

typedef struct stat_s stat_t;

typedef void (*StatCallback)(struct stat_s* self, float old, float cur);
typedef float (*StatGetter)(struct stat_s* self);

typedef struct{
  StatType      type;
  float         min, max, val;
  StatCallback  on_empty, on_full;
}stat_d;

typedef struct{
  const char    name[MAX_NAME_LEN];
  stat_d        stats[STAT_DONE];
}stats_d;

struct stat_s{
  StatType      type;
  float         min;
  float         max;
  float         current;
  float         increment;
  StatCallback  on_change, on_full, on_empty;
};

stat_t* InitStatOnMin(StatType, float min, float max);
stat_t* InitStatOnMax(StatType, float);
stat_t* InitStatEmpty(void);
stat_t* InitStat(StatType, float min, float max, float amount);

#endif
