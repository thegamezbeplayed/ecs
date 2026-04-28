#ifndef __GAME_BEHAVE__
#define __GAME_BEHAVE__
#include "game_common.h"
#include "game_strings.h"

#define DEFINE_BT_LEAF(name) \
static inline behavior_tree_node_t* Leaf##name(behavior_params_t *params) { \
    return BehaviorCreateLeaf(Behavior##name, params); \
}
#define MAX_BEHAVIOR_TREE 12
//<===BEHAVIOR TREES

//forward declare
struct behavior_tree_node_s;

typedef uint64_t BehaviorID;

typedef enum{
  BT_LEAF,
  BT_SEQUENCE,
  BT_SELECTOR,
  BT_CONCURRENT,
  BT_DECIDER,
}BehaviorTreeType;


typedef BehaviorStatus (*BehaviorTreeTickFunc)(struct behavior_tree_node_s* self, void* context);

typedef struct behavior_params_s{
  EntityState           state, o_state;
}behavior_params_t;

typedef struct behavior_tree_node_s{
  BehaviorID            id;
  BehaviorTreeType      bt_type;
  BehaviorTreeTickFunc  tick;
  void*                 data;
}behavior_tree_node_t;

typedef struct{
  behavior_tree_node_t* bt;
  BehaviorID              current, failure;
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

typedef BehaviorStatus (*BehaviorTreeLeafFunc)(behavior_params_t* params);

typedef struct{
  BehaviorTreeLeafFunc  action;
  behavior_params_t*    params;
}behavior_tree_leaf_t;

behavior_tree_node_t* InitBehaviorTree(BehaviorID);
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
  bool                 is_root;
  BehaviorTreeType     bt_type;
  behavior_tree_node_t *(*func)(behavior_params_t *);
  int                  num_children;
  char*                children[5];
} behavior_d;


#endif
