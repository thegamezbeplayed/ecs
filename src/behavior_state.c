#include "behavior_define.h"
#include "component_define.h"

bool BehaviorInit(void* comp, component_entry_t* j){
  behavior_t* b = comp;

  return ParseBehaviorComponent(j->data, b);
}

bool StateInit(void* comp, component_entry_t* j){
  state_t* s = comp;

  s->old = STATE_NONE;
  s->state = STATE_NONE;

  return true;
}

bool BehaviorCanChangeState(State, State){

}
