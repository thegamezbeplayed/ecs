#ifndef __GAME_BEHAVE__
#define __GAME_BEHAVE__
#include "game_utils.h"
#include "game_strings.h"

#define DEFINE_BT_LEAF(name) \
static inline behavior_tree_node_t* Leaf##name(behavior_params_t *params) { \
    return BehaviorCreateLeaf(Behavior##name, params); \
}
#define MAX_BEHAVIOR_TREE 12
//<===BEHAVIOR TREES
typedef uint64_t BehaviorID;

typedef enum{
  STATE_NONE,//if ent_t is properly initalized to {0} this is already set
  STATE_SPAWN,//Should only be set after NONE
  STATE_IDLE, //should be able to move freely between these ==>
  STATE_AGGRO,
  STATE_DIE,//<===== In MOST cases. Should not be able to go down from DIE
  STATE_END,//sentinel entity state should never be this or greater
}State;

typedef struct{
  State       state, old;
  BehaviorID  behaviors[STATE_END]; 
}state_t;

static state_change_requirement_t CAN_CHANGE[STATE_END+1] = {
  {STATE_NONE, NEVER, STATE_END},
  {STATE_SPAWN, LESS_THAN, STATE_SPAWN},
  {STATE_IDLE, LESS_THAN, STATE_DIE},
  {STATE_DIE, LESS_THAN, STATE_DIE},
  {STATE_END, EQUAL_TO, STATE_DIE},
};

//forward declare
struct behavior_tree_node_s;

typedef enum{
  BT_LEAF,
  BT_SEQUENCE,
  BT_SELECTOR,
  BT_CONCURRENT,
  BT_DECIDER,
}BehaviorTreeType;

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


typedef struct{
  hash_map_t              map;
  int                     count;
  BehaviorID              *list;
  BehaviorID              current, failure;
}behavior_t;
static behavior_tree_node_t* BehaviorGetEntityNode(behavior_t* b, BehaviorID id){
  return HashGet(&b->map, id);
} 
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

typedef struct {
  char                 name[MAX_NAME_LEN];
  State                state;
  BehaviorTreeType     bt_type;
  behavior_tree_node_t *(*func)(behavior_params_t *);
  int                  num_children;
  char*                children[5];
  bool                 param_overide;
  int                  o_state;
} behavior_d;

BehaviorStatus BehaviorChangeState(behavior_params_t *params);
static inline behavior_tree_node_t* LeafChangeState(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorChangeState,params); }

BehaviorStatus BehaviorCheckAggro(behavior_params_t *params);
static inline behavior_tree_node_t* LeafCheckAggro(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorCheckAggro,params); }


BehaviorStatus BehaviorAcquireDestination(behavior_params_t *params);
static inline behavior_tree_node_t* LeafAcquireDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorAcquireDestination,params); }

BehaviorStatus BehaviorMoveToDestination(behavior_params_t *params);
static inline behavior_tree_node_t* LeafMoveToDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorMoveToDestination,params); }


#endif
