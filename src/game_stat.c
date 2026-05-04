#include "game_stats.h"
#include "game_tools.h"

stat_t* InitStatOnMin(StatType, float min, float max){

}

stat_t* InitStatOnMax(StatType, float){

}

stat_t* InitStatEmpty(void){

}

stat_t* InitStat(StatType type, float min, float max, float amount){
  stat_t* s = GameCalloc("InitStat", 1, sizeof(stat_t));

  s->type = type;
  s->min = min;
  s->max = max;
  s->current = amount;

  return s;
}
