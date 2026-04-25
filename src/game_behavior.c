#include <raylib.h>
#include "game_behaviors.h"

tree_cache_t tree_cache[18];
int tree_cache_count;

behavior_tree_node_t *BehaviorGetTree(BehaviorID id) {
   for (int i = 0; i < tree_cache_count; i++){
    if (tree_cache[i].id == id)
      return tree_cache[i].root;
  }

   return NULL;
}

behavior_tree_node_t *BuildTreeNode(BehaviorID id,behavior_params_t* parent_params) {
  /*
  if (room_behaviors[id].id != id)
    return NULL;

  BehaviorData data = room_behaviors[id];
  if(data.param_overide || parent_params == NULL){
    parent_params = malloc(sizeof(behavior_params_t));
    *parent_params =(behavior_params_t){
        .state = data.state,
    };
  }

  behavior_tree_node_t *out = NULL;
  if(data.bt_type == BT_LEAF)
    out = room_behaviors[id].func(parent_params);
  else{
    behavior_tree_node_t **kids = GameCalloc("BuildTreeNode", 1,sizeof(*kids) * data.num_children);
    for (int j = 0; j < data.num_children; ++j)
      kids[j] = BuildTreeNode(data.children[j],parent_params);

    switch(data.bt_type){
      case BT_SEQUENCE:
        out = BehaviorCreateSequence(kids, data.num_children);
        break;
      case BT_SELECTOR:
        out = BehaviorCreateSelector(kids, data.num_children);
        break;
      case BT_CONCURRENT:
        out = BehaviorCreateConcurrent(kids, data.num_children);
        break;
      default:
        TraceLog(LOG_WARNING,"Behavior Node Type %d NOT FOUND!",data.bt_type);
        return NULL;
        break;
    }

  }

  out->id = id;

  return out;
*/
}

behavior_tree_node_t* InitBehaviorTree( BehaviorID id){
  if(id ==BN_NONE)
    return NULL;
  behavior_tree_node_t* node = BehaviorGetTree(id);

  if(node != NULL)
    return node;

  TraceLog(LOG_WARNING,"<=====Behavior Tree %i not found=====>",id);
  return NULL;
}


BehaviorStatus BehaviorTickLeaf(behavior_tree_node_t *self, void *context) {
    
  behavior_tree_leaf_t *leaf = (behavior_tree_leaf_t *)self->data;
    if (!leaf || !leaf->action)
      return BEHAVIOR_FAILURE;
    BehaviorStatus status = leaf->action(leaf->params);

    return status;
}

behavior_tree_node_t* BehaviorCreateLeaf(BehaviorTreeLeafFunc fn, behavior_params_t* params){
  behavior_tree_leaf_t *data = GameCalloc("BehaviorCreateLeaf", 1, sizeof(behavior_tree_leaf_t));

  data->action = fn;
  data->params = params;
  
  behavior_tree_node_t* node = GameCalloc("BehaviorCreateLeaf node", 1, sizeof(behavior_tree_node_t));
  node->bt_type = BT_LEAF;
  node->tick = BehaviorTickLeaf;
  node->data = data;

  return node;
}

behavior_tree_node_t* BehaviorCreateSequence(behavior_tree_node_t **children, int count) {
    behavior_tree_sequence_t *data = GameCalloc("BehaviorCreateSequence", 1,sizeof(behavior_tree_sequence_t));
    data->children = children;
    data->num_children = count;
    data->current = 0;

    behavior_tree_node_t *node = GameCalloc("BehaviorCreateSequence node", 1, sizeof(behavior_tree_node_t));
    node->bt_type = BT_SEQUENCE;
    node->tick = BehaviorTickSequence;
    node->data = data;
    return node;
}

behavior_tree_node_t* BehaviorCreateSelector(behavior_tree_node_t **children, int count) {
    behavior_tree_selector_t *data = GameCalloc("BehaviorCreateSelector", 1, sizeof(behavior_tree_selector_t));
    data->children = children;
    data->num_children = count;
    data->current = 0;

    behavior_tree_node_t *node = GameCalloc("BehaviorCreateSelector node", 1, sizeof(behavior_tree_node_t));
    node->bt_type = BT_SELECTOR;
    node->tick = BehaviorTickSelector;
    node->data = data;
    return node;
}

behavior_tree_node_t* BehaviorCreateConcurrent(behavior_tree_node_t **children, int count) {
    behavior_tree_selector_t *data = GameCalloc("BehaviorCreateConcurrent", 1, sizeof(behavior_tree_selector_t));
    data->children = children;
    data->num_children = count;
    data->current = 0;

    behavior_tree_node_t *node = GameCalloc("BehaviorCreateConcurrent node", 1, sizeof(behavior_tree_node_t));
    node->bt_type = BT_CONCURRENT;
    node->tick = BehaviorTickConcurrent;
    node->data = data;
    return node;
}

BehaviorStatus BehaviorTickSequence(behavior_tree_node_t *self, void *context) {
  behavior_tree_sequence_t *seq = (behavior_tree_sequence_t *)self->data;
  while (seq->current < seq->num_children) {
    BehaviorStatus status = seq->children[seq->current]->tick(seq->children[seq->current], context);
    if (status == BEHAVIOR_RUNNING) return BEHAVIOR_RUNNING;
    if (status == BEHAVIOR_FAILURE) {
      seq->current = 0;
      return BEHAVIOR_FAILURE;
    }
    seq->current++;
  }

  seq->current = 0;
  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorTickSelector(behavior_tree_node_t *self, void *context) {
  behavior_tree_selector_t *sel = (behavior_tree_selector_t *)self->data;

  while (sel->current < sel->num_children) {
    BehaviorStatus status = sel->children[sel->current]->tick(sel->children[sel->current], context);
    if (status == BEHAVIOR_RUNNING) return BEHAVIOR_RUNNING;
    if (status == BEHAVIOR_SUCCESS) {
      sel->current = 0;
      return BEHAVIOR_SUCCESS;
    }
    sel->current++;
  }

  sel->current = 0;
  return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorTickConcurrent(behavior_tree_node_t *self, void *context) {
  behavior_tree_selector_t *sel = (behavior_tree_selector_t *)self->data;

  bool anyRunning = false;
  bool anySuccess = false;
  bool anyFailure = false;

  for (int i = 0; i < sel->num_children; i++) {
    BehaviorStatus status = sel->children[i]->tick(sel->children[i], context);
    if (status == BEHAVIOR_RUNNING) anyRunning = true;
    else if (status == BEHAVIOR_SUCCESS) anySuccess = true;
    else if (status == BEHAVIOR_FAILURE) anyFailure = true;
  }

  // Rule set: "success if all succeed"
  if (!anyRunning && !anyFailure) return BEHAVIOR_SUCCESS;
  if (anyRunning) return BEHAVIOR_RUNNING;
  return BEHAVIOR_FAILURE;
}

