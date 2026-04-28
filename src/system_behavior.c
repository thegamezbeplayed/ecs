#include "game_register.h"
#include "game_systems.h"

#include "game_behaviors.h"
#include "data_behaviors.h"

behavior_d* BehaviorGetEntry(behavior_st* b, BehaviorID key){
  return HashGet(&b->map, key);
}

behavior_tree_node_t *BuildTreeNode(behavior_st* b, BehaviorID id) {
  behavior_d* data = BehaviorGetEntry(b, id);
  if(!data)
    return NULL;

  behavior_tree_node_t *out = NULL;
  if(data->bt_type == BT_LEAF){

    out = data->func(NULL);
  }
  else{
    behavior_tree_node_t **kids = GameCalloc("BuildTreeNode", 1,sizeof(*kids) * data->num_children);
    for (int j = 0; j < data->num_children; ++j){
      BehaviorID cid = hash_str_64(data->children[j]);
      kids[j] = BuildTreeNode(b, cid);
    }

    switch(data->bt_type){
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
        TraceLog(LOG_WARNING,"Behavior Node Type %d NOT FOUND!",data->bt_type);
        return NULL;
        break;
    }

  }

  out->id = id;

  return out;

}

void LoadBehaviors(system_pool_t* s, component_registry_t* c){
  behavior_st* b = &s->behavior;

  for(int i = 0; i < BN_COUNT; i++){
    behavior_d data = BEHAVE[i];
    BehaviorID id = hash_str_64(data.name);
    HashPut(&b->map, id, &data);

  }

  hash_iter_t iter;
  HashStart(&b->map, &iter);

  hash_slot_t* hs;

  while((hs = HashNext(&iter))){
    behavior_d *data = hs->value;

    if(!data->is_root)
      continue;

    BuildTreeNode(b, hs->key);
  }
}

void InitBehaviorSystem(behavior_st* b){
  HashInit(&b->map, next_pow2_int(BN_COUNT*2));
  HashInit(&b->factory, next_pow2_int(BN_COUNT*2));
  RegisterScheduleState(GAME_LOADING, LoadBehaviors);
}

