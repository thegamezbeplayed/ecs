#include "stat_define.h"
#include "game_define.h"
#include "component_define.h"

bool StatInit(void* comp, component_entry_t* j){
  stat_t* s = comp;

  return ParseStatComponent(j->data, s);
}
