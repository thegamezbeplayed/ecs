#ifndef __GAME_BEHAVE__
#define __GAME_BEHAVE__
#include "game_utils.h"
#include "game_register.h"
#include "game_strings.h"
#include "behavior_enum.h"

#define DEFINE_BT_LEAF(name) \
static inline behavior_tree_node_t* Leaf##name(behavior_params_t *params) { \
    return BehaviorCreateLeaf(Behavior##name, params); \
}
#define MAX_BEHAVIOR_TREE   12
#define MAX_BEHAVIOR_CHILD  5
//<===BEHAVIOR TREES
typedef uint64_t BehaviorID;
extern BehaviorID INVALID_BEHAVIOR;
typedef struct{
  State       state, old;
//  BehaviorID  behaviors[STATE_END]; 
}state_t;

bool BehaviorSetState(state_t*, State);
bool BehaviorCanChangeState(State, State);

static state_change_requirement_t CAN_CHANGE[STATE_END+1] = {
  {STATE_NONE, NEVER, STATE_END},
  {STATE_SPAWN, LESS_THAN, STATE_SPAWN},
  {STATE_IDLE, LESS_THAN, STATE_DIE},
  {STATE_AGGRO, LESS_THAN, STATE_DIE},
  {STATE_ATTACK, LESS_THAN, STATE_DIE},
  {STATE_DIE, LESS_THAN, STATE_DIE},
  {STATE_END, EQUAL_TO, STATE_DIE},
};

//forward declare
struct behavior_tree_node_s;

typedef BehaviorStatus (*BehaviorTreeTickFunc)(struct behavior_tree_node_s* self, void*);

typedef struct behavior_params_s{
  State           state, old;
  Entity          *ent;
}behavior_params_t;

typedef struct behavior_tree_node_s{
  char                  name[MAX_NAME_LEN];
  BehaviorID            id;
  BehaviorTreeType      bt_type;
  BehaviorTreeTickFunc  tick;
  void*                 data;
}behavior_tree_node_t;
extern hash_map_t BEHAVIOR_TREE;
void BehaviorTreeInit(int cap);
BehaviorID BehaviorTreeGetID(const char* name);
behavior_tree_node_t* InitBehaviorTree(BehaviorID id);
behavior_tree_node_t* BehaviorGetNode(BehaviorID id);

typedef struct{
  char            name[MAX_NAME_LEN];
  State           state;
  BehaviorID      id, current, failure;
  BehaviorStatus  last_run;
}behavior_t;

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

typedef BehaviorStatus (*BehaviorTreeLeafFunc)(world_t*, behavior_params_t* params);
typedef behavior_tree_node_t* (*BehaviorLeafInit)(behavior_params_t *);

typedef struct{
  char                name[MAX_NAME_LEN];
  BehaviorID          id;
  BehaviorTreeType    type;
  BehaviorLeafInit    fn;
  bool                param_overide;
  State               state;
  bool                is_root;
  int                 num_children;
  BehaviorID          children[MAX_BEHAVIOR_CHILD];
}behavior_define_t;

extern hash_map_t BEHAVIOR_DEFS;
void BehaviorDefInit(int cap);
void RegisterBehaviorDef(behavior_define_t* b);
void BuildBehaviorRoots(void);

typedef struct{
  BehaviorTreeLeafFunc  action;
  behavior_params_t*    params;
}behavior_tree_leaf_t;

BehaviorStatus BehaviorTickSequence(behavior_tree_node_t *self, void *context);
BehaviorStatus BehaviorTickSelector(behavior_tree_node_t *self, void *context);
BehaviorStatus BehaviorTickConcurrent(behavior_tree_node_t *self, void *context);
behavior_tree_node_t* BehaviorCreateLeaf(BehaviorTreeLeafFunc fn, behavior_params_t* params);
behavior_tree_node_t* BehaviorCreateSequence(behavior_tree_node_t **children, int count);
behavior_tree_node_t* BehaviorCreateSelector(behavior_tree_node_t **children, int count);
behavior_tree_node_t* BehaviorCreateConcurrent(behavior_tree_node_t **children, int count);

BehaviorStatus BehaviorChangeState(world_t*, behavior_params_t *params);
static inline behavior_tree_node_t* LeafChangeState(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorChangeState,params); }

BehaviorStatus BehaviorCheckAggro(world_t*, behavior_params_t *params);
static inline behavior_tree_node_t* LeafCheckAggro(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorCheckAggro,params); }

BehaviorStatus BehaviorAttack(world_t*, behavior_params_t *params);
static inline behavior_tree_node_t* LeafAttack(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorAttack,params); }

BehaviorStatus BehaviorAcquireDestination(world_t*, behavior_params_t *params);
static inline behavior_tree_node_t* LeafAcquireDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorAcquireDestination,params); }

BehaviorStatus BehaviorMoveToDestination(world_t*, behavior_params_t *params);
static inline behavior_tree_node_t* LeafMoveToDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorMoveToDestination,params); }

BehaviorStatus BehaviorMoveToTarget(world_t*, behavior_params_t *params);
static inline behavior_tree_node_t* LeafMoveToTarget(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorMoveToTarget,params); }

#endif
