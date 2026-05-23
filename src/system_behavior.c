#include "behavior_define.h"
#include "system_define.h"

void BehaviorRegister(world_t* w, Entity e){

}

void BehaviorSystem(world_t* w, Entity e){
  state_t* s = GET_COMPONENT(w, e, state_t, STATE_ID);

}

void BehaviorLoad(world_t* w){
  BehaviorTreeInit(MAX_BEHAVIOR_TREE);
  BehaviorDefInit(MAX_BEHAVIOR_TREE * MAX_BEHAVIOR_CHILD);
}
