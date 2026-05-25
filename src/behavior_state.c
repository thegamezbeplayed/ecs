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

bool BehaviorSetState(state_t* bs, State s){
  if(bs->state == s)
    return false;

  if (!BehaviorCanChangeState(bs->state, s))
    return false;

  bs->state = s;

  return bs->state == s;
}

bool BehaviorCanChangeState(State cur, State s){
  state_change_requirement_t check = CAN_CHANGE[s];

  return check.can(cur, s);
}
