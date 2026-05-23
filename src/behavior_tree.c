#include "behavior_define.h"

hash_map_t BEHAVIOR_TREE;
hash_map_t BEHAVIOR_DEFS;
BehaviorID INVALID_BEHAVIOR = 0;

void RegisterBehaviorDef(behavior_define_t* b){
  b->id = hash_str_64(b->name);
  HashPut(&BEHAVIOR_DEFS, b->id, b);
}

void BehaviorDefInit(int cap){
  HashInit(&BEHAVIOR_DEFS, next_pow2_int(cap));
}

void BehaviorTreeInit(int cap){
  HashInit(&BEHAVIOR_TREE, next_pow2_int(cap));
}

void BehaviorTreeAddNode(const char* name, behavior_tree_node_t* node){
  node->id = hash_str_64(name);

  HashPut(&BEHAVIOR_TREE, node->id, node);
}

behavior_define_t* BehaviorGetDef(BehaviorID id){
  return HashGet(&BEHAVIOR_DEFS, id);
}

behavior_tree_node_t* BehaviorGetNode(BehaviorID id){
  return HashGet(&BEHAVIOR_TREE, id);
}

BehaviorID BehaviorTreeGetID(const char* name){
  BehaviorID id = hash_str_64(name);

  if(BehaviorGetNode(id) != NULL)
    return id;

  return INVALID_BEHAVIOR;
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

behavior_tree_node_t *BuildTreeNode(behavior_define_t* data, behavior_params_t* parent_params) {
  if(data->param_overide || parent_params == NULL){
    parent_params = GameMalloc("BuildTreeNode", sizeof(behavior_params_t));
    *parent_params =(behavior_params_t){
        .state = data->state,
    };
  }
  
  behavior_tree_node_t *out = NULL;
  if(data->type == BT_LEAF)
    out = data->fn(parent_params);
  else{
    behavior_tree_node_t **kids = GameCalloc("BuildTreeNode", 1,sizeof(*kids) * data->num_children);
    for (int j = 0; j < data->num_children; ++j){
      behavior_define_t* child = BehaviorGetDef(data->children[j]);
      kids[j] = BuildTreeNode(child, parent_params);
    }
    switch(data->type){
      case BT_SEQUENCE:
        out = BehaviorCreateSequence(kids, data->num_children);
        break;
      case BT_SELECTOR:
        out = BehaviorCreateSelector(kids, data->num_children);
        break;
      case BT_CONCURRENT:
        out = BehaviorCreateConcurrent(kids, data->num_children);
        break;
      default:
        TraceLog(LOG_WARNING,"Behavior Node Type %d NOT FOUND!",data->type);
        return NULL;
        break;
    }
  }

  strcpy(out->name, data->name);
  out->id = data->id;

  return out;
}

void BuildBehaviorRoots(void){
  hash_iter_t iter;
  HashStart(&BEHAVIOR_DEFS, &iter);

  hash_slot_t* s;
  while((s = HashNext(&iter))){
    behavior_define_t* def = s->value;

    if(!def->is_root)
      continue;
      
    BehaviorTreeAddNode(def->name, BuildTreeNode(def, NULL));
  }
}

BehaviorStatus BehaviorTickLeaf(behavior_tree_node_t *self, void *context) {
  world_t* w = WorldGetContext();
  behavior_tree_leaf_t *leaf = (behavior_tree_leaf_t *)self->data;
    if (!leaf || !leaf->action)
      return BEHAVIOR_FAILURE;
   
    leaf->params->ent = context; 
    BehaviorStatus status = leaf->action(w, leaf->params);

    return status;
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

