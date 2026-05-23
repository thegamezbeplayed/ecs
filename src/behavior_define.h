#ifndef __GAME_BEHAVE__
#define __GAME_BEHAVE__
#include "game_utils.h"
#include "game_strings.h"
#include "behavior_enum.h"

#define DEFINE_BT_LEAF(name) \
static inline behavior_tree_node_t* Leaf##name(behavior_params_t *params) { \
    return BehaviorCreateLeaf(Behavior##name, params); \
}
#define MAX_BEHAVIOR_TREE 12
//<===BEHAVIOR TREES
typedef uint64_t BehaviorID;

typedef struct{
  State       state, old;
//  BehaviorID  behaviors[STATE_END]; 
}state_t;

bool BehaviorCanChangeState(State, State);

static state_change_requirement_t CAN_CHANGE[STATE_END+1] = {
  {STATE_NONE, NEVER, STATE_END},
  {STATE_SPAWN, LESS_THAN, STATE_SPAWN},
  {STATE_IDLE, LESS_THAN, STATE_DIE},
  {STATE_DIE, LESS_THAN, STATE_DIE},
  {STATE_END, EQUAL_TO, STATE_DIE},
};

//forward declare
struct behavior_tree_node_s;

typedef BehaviorStatus (*BehaviorTreeTickFunc)(struct behavior_tree_node_s* self, void*);

typedef struct behavior_params_s{
  State           state, old;
  void*           context;
}behavior_params_t;

typedef struct behavior_tree_node_s{
  char                  name[MAX_NAME_LEN];
  BehaviorID            id;
  BehaviorTreeType      bt_type;
  BehaviorTreeTickFunc  tick;
  void*                 data;
}behavior_tree_node_t;
extern hash_map_t BEHAVIOR_TREE;
behavior_tree_node_t* InitBehaviorTree(BehaviorID id);

typedef struct{
  BehaviorID              id, current, failure;
  BehaviorStatus           last_run;
}behavior_t;

/*
static behavior_tree_node_t* BehaviorGetEntityNode(behavior_t* b, BehaviorID id){
  return HashGet(&b->map, id);
} 
*/
behavior_t* InitBehavior(int cap, int count, ...);

typedef struct{
  behavior_tree_node_t  **children;
  int                   num_children;
  int                   current;
}behavior_tree_sequence_t;

typedef struct{
  behavior_tree_node_t  **children;
  int                   num_children;
  int                   current;
}behavior_tree_selector_t;

typedef BehaviorStatus (*BehaviorTreeLeafFunc)(behavior_params_t* params);

typedef struct{
  BehaviorTreeLeafFunc  action;
  behavior_params_t*    params;
}behavior_tree_leaf_t;

void FreeBehaviorTree(behavior_tree_node_t* node);
BehaviorStatus BehaviorTickSequence(behavior_tree_node_t *self, void *context);
BehaviorStatus BehaviorTickSelector(behavior_tree_node_t *self, void *context);
BehaviorStatus BehaviorTickConcurrent(behavior_tree_node_t *self, void *context);
behavior_tree_node_t* BehaviorCreateLeaf(BehaviorTreeLeafFunc fn, behavior_params_t* params);
behavior_tree_node_t* BehaviorCreateSequence(behavior_tree_node_t **children, int count);
behavior_tree_node_t* BehaviorCreateSelector(behavior_tree_node_t **children, int count);
behavior_tree_node_t* BehaviorCreateConcurrent(behavior_tree_node_t **children, int count);

BehaviorStatus BehaviorChangeState(behavior_params_t *params);
static inline behavior_tree_node_t* LeafChangeState(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorChangeState,params); }

BehaviorStatus BehaviorCheckAggro(behavior_params_t *params);
static inline behavior_tree_node_t* LeafCheckAggro(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorCheckAggro,params); }


BehaviorStatus BehaviorAcquireDestination(behavior_params_t *params);
static inline behavior_tree_node_t* LeafAcquireDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorAcquireDestination,params); }

BehaviorStatus BehaviorMoveToDestination(behavior_params_t *params);
static inline behavior_tree_node_t* LeafMoveToDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorMoveToDestination,params); }


#endif
