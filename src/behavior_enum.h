#ifndef __BEHAVE_ENUM__
#define __BEHAVE_ENUM__
#include "game_enum.h"
#define EVENT_BEHAVE_BASE     0xB000

DEFINE_EVENT_SPACE(BehaviorEvent, EVENT_BEHAVE_BASE);

typedef enum{
  BEHAVIOR_EVENT_NONE,
  BEHAVIOR_EVENT_STATE,
  BEHAVIOR_EVENT_COUNT
}BehaviorEventID;

typedef enum{
  STATE_NONE, //if ent_t is properly initalized to {0} this is already set
  STATE_SPAWN, //Should only be set after NONE
  STATE_IDLE, //should be able to move freely between these ==>
  STATE_AGGRO,
  STATE_ATTACK,
  STATE_DIE, //<===== In MOST cases. Should not be able to go down from DIE
  STATE_END, //sentinel entity state should never be this or greater
}State;

typedef enum{
  BT_NONE = -1,
  BT_LEAF,
  BT_SEQUENCE,
  BT_SELECTOR,
  BT_CONCURRENT,
  BT_DECIDER,
}BehaviorTreeType;

#endif
