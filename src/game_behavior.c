#include <raylib.h>
#include "game_behaviors.h"
#include "game_systems.h"

behavior_t* InitBehavior(int cap, int count, ...){
  behavior_t* b = GameCalloc("InitBehavior", 1, sizeof(behavior_t));

  HashInit(&b->map, next_pow2_int(cap*2));

  va_list args;
  va_start(args, count);

  b->list = GameCalloc("InitBehavior", count, sizeof(BehaviorID));
  for (int i = 0; i < count; i++) {
    char* bname = va_arg(args, char*);
    BehaviorID id = hash_str_64(bname);
    BehaviorID *entry = &b->list[b->count++];
    *entry = id;
  }

  return b;
}

BehaviorStatus BehaviorTickLeaf(behavior_tree_node_t *self, void *context) {
    
  behavior_tree_leaf_t *leaf = (behavior_tree_leaf_t *)self->data;
    if (!leaf || !leaf->action)
      return BEHAVIOR_FAILURE;

    leaf->params->context = context;
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


BehaviorStatus BehaviorChangeState(behavior_params_t *params){
behavior_ctx_t* ctx = params->context;
  if(!ctx)
    return BEHAVIOR_FAILURE;

  Entity* e               = ctx->e;
  system_pool_t* s        = ctx->s;
  component_registry_t* c = ctx->c;
  if(!e || !s || !c)
    return BEHAVIOR_FAILURE;


    return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorCheckAggro(behavior_params_t *params){
  behavior_ctx_t* ctx = params->context;
  if(!ctx)
    return BEHAVIOR_FAILURE;

  Entity* e               = ctx->e;
  system_pool_t* s        = ctx->s;
  component_registry_t* c = ctx->c;
  if(!e || !s || !c)
    return BEHAVIOR_FAILURE;


    return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorAcquireDestination(behavior_params_t *params){
  behavior_ctx_t* ctx = params->context;
  if(!ctx)
    return BEHAVIOR_FAILURE;

  Entity* e               = ctx->e;
  system_pool_t* s        = ctx->s;
  component_registry_t* c = ctx->c;
  if(!e || !s || !c)
    return BEHAVIOR_FAILURE;

  position_c* pos = &c->pos;
  int pidx = ComponentByEntity(&pos->map, e->id);
  position_t* p = &pos->dense[pidx];

  if(!vec_compare(p->vdest, VEC_UNSET) && !vec_compare(p->vpos, p->vdest))
    return BEHAVIOR_SUCCESS;

  int range = RandRange(2, 12);

  range*=CELL_WIDTH;
  if(vec_compare(p->dir, VEC_UNSET))
    p->dir = VECTOR2_LEFT;

  Vector2 step = Vector2Scale(p->dir, range);

  PositionSetDest(p, Vector2Add(p->vpos, step));

  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorMoveToDestination(behavior_params_t *params){
  behavior_ctx_t* ctx = params->context;
  if(!ctx)
    return BEHAVIOR_FAILURE;

  Entity* e               = ctx->e;
  system_pool_t* s        = ctx->s;
  component_registry_t* c = ctx->c;
  if(!e || !s || !c)
    return BEHAVIOR_FAILURE;

  position_c* pos = &c->pos;
  int pidx = ComponentByEntity(&pos->map, e->id);
  position_t* p = &pos->dense[pidx];

  if(vec_dist(p->vpos, p->vdest)<1){
    p->vdest = VEC_UNSET;
    return BEHAVIOR_SUCCESS;
  }

  GameEvent("DEST_SET",  p, e->id);

  return BEHAVIOR_RUNNING;
}
