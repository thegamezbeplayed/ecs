#include "behavior_define.h"
#include "system_define.h"
#include "util_parse.h"
#include "system_events.h"

void BehaviorRegister(world_t* w, Entity e){
  behavior_t* b = GET_COMPONENT(w, e, behavior_t, BEHAVE_ID);

  b->id = BehaviorTreeGetID(b->name);

  if(b->id == INVALID_BEHAVIOR)
    TraceLog(LOG_WARNING, "=== BEHAVIOR REGISTER ===\n Invalid Behavior %s", b->name);
}

void BehaviorSystem(world_t* w, Entity e){
  behavior_t* b = GET_COMPONENT(w, e, behavior_t, BEHAVE_ID);

  if(!EntityHasRelation(w, e, "BehaviorOf"))
    return;

  Entity rel = EntityGetRelationTarget(w, e, "BehaviorOf");

  state_t* s = GET_COMPONENT(w, rel, state_t, STATE_ID);

  if(!s || s->state != b->state)
    return;

  behavior_tree_node_t* node = BehaviorGetNode(b->id);
  
  if(!node)
    return;

  b->last_run = node->tick(node, &rel);

}

void BehaviorLoad(world_t* w){
  BehaviorTreeInit(MAX_BEHAVIOR_TREE);
  BehaviorDefInit(MAX_BEHAVIOR_TREE * MAX_BEHAVIOR_CHILD);

  cJSON* root = ParseRoot("resources/data/behavior_def.json");
  ParseBehaviorDefs(root);

  BuildBehaviorRoots();
  HashFree(&BEHAVIOR_DEFS);
}

void StateSink(void* obs_data, void* sub, payload_t* pl){
  state_t* st = obs_data;

  switch(EVENT_ID(pl->event)){
    case ANIM_EVENT_SEQ_END:
    case ANIM_EVENT_SUSPEND:
      if(pl->type_id != ANIM_ID){
        TraceLog(LOG_WARNING, "=== STATE SINK BAD DATA\n Expected ANIM ID %i but payload is type %i",
            ANIM_ID, pl->type_id);
        return;
      }
      StateHandleAnim(st, pl->data);
      break;
    default:
      TraceLog(LOG_INFO, "=== STATE SINK ===\n Some Other Event recieved");
      break;
  }
}

void StateBegin(world_t* w, Entity e){
  state_t* s = GET_COMPONENT(w, e, state_t, STATE_ID);

  BehaviorSetState(s, STATE_SPAWN);
}

void StateRegister(world_t* w){
  LookAddSink("State", StateSink);

}
