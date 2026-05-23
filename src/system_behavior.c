#include "behavior_define.h"
#include "system_define.h"
#include "util_parse.h"


void BehaviorRegister(world_t* w, Entity e){
  behavior_t* b = GET_COMPONENT(w, e, behavior_t, BEHAVE_ID);

  b->id = BehaviorTreeGetID(b->name);

  if(b->id == INVALID_BEHAVIOR)
    TraceLog(LOG_WARNING, "=== BEHAVIOR REGISTER ===\n Invalid Behavior %s", b->name);
}

void BehaviorSystem(world_t* w, Entity e){
  behavior_t* b = GET_COMPONENT(w, e, behavior_t, BEHAVE_ID);

  if(!EntityHasRelation(w, e, REL_BehaviorOf))
    return;

  Entity rel = EntityGetRelationTarget(w, e, REL_BehaviorOf);

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

void StateBegin(world_t* w, Entity e){
  state_t* s = GET_COMPONENT(w, e, state_t, STATE_ID);

  BehaviorSetState(s, STATE_SPAWN);
}
