#include "behavior_define.h"

hash_map_t BEHAVIOR_TREE;


void BehaviorTreeInit(int cap){
  HashInit(&BEHAVIOR_TREE, next_pow2_int(cap));
}

void BehaviorTreeAddNode(const char* name, behavior_tree_node_t* node){
  node->id = hash_str_64(name);

  HashPut(&BEHAVIOR_TREE, node->id, node);
}

behavior_tree_node_t* BehaviorGetNode(BehaviorID id){
  return HashGet(&BEHAVIOR_TREE, id);
}

behavior_tree_node_t* InitBehaviorTree(BehaviorID id){
  if(id < 1)
    return NULL;
  behavior_tree_node_t* node = BehaviorGetNode(id);
  if(node != NULL)
    return node;
  TraceLog(LOG_WARNING,"<=====Behavior Tree %i not found=====>",id);
  return NULL;
}

behavior_tree_node_t *BuildTreeNode(BehaviorID id,behavior_params_t* parent_params) {
  BehaviorData data = room_behaviors[id];
  if(data.param_overide || parent_params == NULL){
    parent_params = malloc(sizeof(behavior_params_t));
    *parent_params =(behavior_params_t){
      .owner = NULL,
        .state = data.state,
        .o_state = data.state,
        .o_action = ACTION_NONE,
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

}


behavior_t* InitBehavior(int cap, int count, ...){

}
