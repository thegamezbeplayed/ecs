#include "game_register.h"
#include "game_systems.h"

#include "game_behaviors.h"
#include "data_behaviors.h"

void OnBehaviorEvent(event_t* ev, void* data){
 position_t* p = ev->data;
 rigid_body_t* rb = data;

 Vector2 dir = Vector2Normalize(p->vdest);
 RigidBodySteer(rb, dir);
}

behavior_d* BehaviorGetEntry(behavior_st* b, BehaviorID key){
  return HashGet(&b->map, key);
}

behavior_tree_node_t* BehaviorGetNode(behavior_st* b, BehaviorID key){
  return HashGet(&b->nodes, key);
}

behavior_tree_node_t* InitBehaviorTree(behavior_st* b, BehaviorID id){
  if(id < 1)
    return NULL;
  behavior_tree_node_t* node = BehaviorGetNode(b, id);

  if(node != NULL)
    return node;

  TraceLog(LOG_WARNING,"<=====Behavior Tree %i not found=====>",id);
  return NULL;

}

behavior_tree_node_t *BuildTreeNode(behavior_st* b, BehaviorID id, behavior_params_t* parent_params) {
  behavior_d* data = BehaviorGetEntry(b, id);
  if(!data)
    return NULL;

  if(data->param_overide || parent_params == NULL){
    parent_params = GameCalloc("BuildTreeNode", 1, sizeof(behavior_params_t));
    *parent_params =(behavior_params_t){
        .state = data->o_state,
    };
  }

  behavior_tree_node_t *out = NULL;
  if(data->bt_type == BT_LEAF){
    out = data->func(parent_params);
  }
  else{
    behavior_tree_node_t **kids = GameCalloc("BuildTreeNode", 1,sizeof(*kids) * data->num_children);
    for (int j = 0; j < data->num_children; ++j){
      BehaviorID cid = hash_str_64(data->children[j]);
      kids[j] = BuildTreeNode(b, cid, parent_params);
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

  strcpy(out->name, data->name);
  out->id = id;

  return out;

}

void BehaviorSystem(system_pool_t* s, component_registry_t* c){
  behavior_st* st = &s->behavior;
  behavior_c*  b = &c->behaviors;
  state_c*     sc = &c->states;
  EntityManager *em = &c->manager;

  for(int i = 0; i < sc->map.size; i++){
    state_t* state = &sc->dense[i];
    Entity* e = ComponentGetEntity(em, &sc->map, i);
    int bid = ComponentByEntity(&b->map, e->id);
    behavior_t* bh = &b->dense[bid];

    if(!bh)
      continue;

    BehaviorID bhid = state->behaviors[state->state];

    behavior_tree_node_t* btn = BehaviorGetEntityNode(bh, bhid);
    if(!btn)
      continue;

    bh->current = bhid;
    behavior_ctx_t* ctx = GameCalloc("BehaviorSystem", 1, sizeof(behavior_ctx_t));

    ctx->e = e;
    ctx->c = c;
    ctx->s = s;

    BehaviorStatus res = btn->tick(btn, ctx);

    if(res == BEHAVIOR_FAILURE)
      bh->failure = bhid;

    GameFree("BehaviorSystem", ctx);
  }

}

void LoadBehaviors(system_pool_t* s, component_registry_t* c){
  behavior_st* b = &s->behavior;

  for(int i = 0; i < BN_COUNT; i++){
    behavior_d data = BEHAVE[i];
    BehaviorID id = hash_str_64(data.name);
    HashPut(&b->map, id, &BEHAVE[i]);

  }

  hash_iter_t iter;
  HashStart(&b->map, &iter);

  hash_slot_t* hs;

  while((hs = HashNext(&iter))){
    behavior_d *data = hs->value;

    if(data->state == 0)
      continue;

    behavior_tree_node_t* btn = BuildTreeNode(b, hs->key, NULL);

    HashPut(&b->nodes, hs->key, btn);
  }
}

void AssignBehaviors(system_pool_t* s, component_registry_t* c){
  behavior_st*    st   = &s->behavior;
  behavior_c*     b    = &c->behaviors;
  state_c*        sc   = &c->states;
  EntityManager*  em   = &c->manager;
  rigid_body_c*   rb   = &c->bodies;
  for(int i = 0; i <  b->map.size; i++){
    behavior_t* be = &b->dense[i];
    Entity* e = ComponentGetEntity(em, &b->map, i);
    int sid = ComponentByEntity(&sc->map, e->id);
    int rid = ComponentByEntity(&rb->map, e->id);

    rigid_body_t* body = &rb->dense[rid];
    if(body)
      SubscribeEntity("DEST_SET", OnBehaviorEvent, body, e->id);

    state_t* state = &sc->dense[sid];
    for(int j = 0; j < be->count; j++){
      behavior_tree_node_t* btn = InitBehaviorTree(st, be->list[j]);

      behavior_d* data = BehaviorGetEntry(st, be->list[j]);
      state->behaviors[data->state] = be->list[j];
      HashPut(&be->map, be->list[j], btn);
    }

    state->state = STATE_SPAWN;
  }
}
void RunBehaviors(system_pool_t* s, component_registry_t* c){
  state_c*     sc   = &c->states;
  for(int i = 0; i <  sc->map.size; i++){
    state_t* state = &sc->dense[i];
    state->old = state->state;
    state->state = STATE_IDLE;
  }

}

void InitBehaviorSystem(behavior_st* b){
  HashInit(&b->map, next_pow2_int(BN_COUNT*2));
  HashInit(&b->nodes, next_pow2_int(BN_COUNT*2));
  RegisterScheduleState(GAME_LOADING, LoadBehaviors);
  RegisterScheduleState(GAME_READY, AssignBehaviors);
  RegisterScheduleState(GAME_RUNNING, RunBehaviors);
  RegisterScheduleStep(UPDATE_PRE, BehaviorSystem);

}

