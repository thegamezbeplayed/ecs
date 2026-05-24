#include "behavior_define.h"

BehaviorStatus BehaviorMoveToDestination(world_t* w, behavior_params_t *params){
  Entity e = *params->ent;
  if(!EntityValid(&w->manager, e))
    return BEHAVIOR_FAILURE;

  position_t* p = ComponentGet(w, e, POS_ID);
  if(!p)
    return BEHAVIOR_FAILURE;

  if(vec_compare(p->dest, VEC_UNSET))
    return BEHAVIOR_FAILURE;

  if(VEC_DIST(p->pos, p->dest) < 12){
    p->dest = VEC_UNSET;
    return BEHAVIOR_SUCCESS;
  }

  p->dir_step = vec_dir_between(p->pos, p->dest);
  ComponentUpdate(w, e, POS_ID);
  return BEHAVIOR_RUNNING;
}

BehaviorStatus BehaviorAcquireDestination(world_t* w, behavior_params_t *params){
  Entity e = *params->ent;
  if(!EntityValid(&w->manager, e))
    return BEHAVIOR_FAILURE;

  position_t* p = ComponentGet(w, e, POS_ID);
  if(!p)
    return BEHAVIOR_FAILURE;

  if(!vec_compare(p->dest, VEC_UNSET))
    return BEHAVIOR_SUCCESS;

  Cell dir = random_direction();

  p->dest = cell_to_vec(dir, rand_range_float(8, 64));
  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorCheckAggro(world_t* w, behavior_params_t *params){
  Entity e = *params->ent;
  if(!EntityValid(&w->manager, e))
    return BEHAVIOR_FAILURE;

  if(EntityHasRelation(w, e, REL_Target)){
    Entity rel = EntityGetRelationTarget(w, e, REL_Target);
    if(EntityValid(&w->manager, rel))
      return BEHAVIOR_SUCCESS;
  }


}

BehaviorStatus BehaviorChangeState(world_t* w, behavior_params_t *params){
  Entity e = *params->ent;
  if(!EntityValid(&w->manager, e))
    return BEHAVIOR_FAILURE;

  state_t* s = ComponentGet(w, e, STATE_ID);
  if(!s)
    return BEHAVIOR_FAILURE;

  if(BehaviorSetState(s, params->state))
    return BEHAVIOR_SUCCESS;

  return BEHAVIOR_FAILURE;
}
