#include "behavior_define.h"
#include "team_define.h"
#include "util_hash.h"


BehaviorStatus BehaviorMoveToTarget(world_t* w, behavior_params_t *params){
  Entity e = *params->ent;
  if(!EntityValid(&w->manager, e))
    return BEHAVIOR_FAILURE;

  position_t* p = ComponentGet(w, e, POS_ID);
  if(!p)
    return BEHAVIOR_FAILURE;

  if(!EntityHasRelation(w, e, REL_TargetOf))
    return BEHAVIOR_FAILURE;

  Entity rel = EntityGetRelationTarget(w, e, REL_TargetOf);
  if(!EntityValid(&w->manager, rel))
    return BEHAVIOR_FAILURE;

  position_t* tp = ComponentGet(w, rel, POS_ID);
  if(VEC_DIST(p->pos, tp->pos) < 12)
    return BEHAVIOR_SUCCESS;

  p->dir_step = vec_dir_between(p->pos, tp->pos);
  ComponentUpdate(w, e, POS_ID);
  return BEHAVIOR_RUNNING;
}

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

  if(EntityHasRelation(w, e, REL_TargetOf)){
    Entity rel = EntityGetRelationTarget(w, e, REL_TargetOf);
    if(EntityValid(&w->manager, rel))
      return BEHAVIOR_SUCCESS;
  }

  position_t* p = ComponentGet(w, e, POS_ID);
  team_t* t = ComponentGet(w, e, TEAM_ID);
  Entity tar = INVALID_ENTITY;

  neighbor_list_t list = SpatialHashGridGetNeighbors(&w->grid, p->pos, 80.0f, e.id);
  for (int i = 0; i < list.count; i++) {
    uint32_t id = list.items[i].id;
    team_t* other = ComponentGetByID(w, id, TEAM_ID);
    if (!other) continue;

    if (t->id == other->id)
      continue;

    float dx = list.items[i].pos.x - p->pos.x;
    float dy = list.items[i].pos.y - p->pos.y;

    if (dx*dx + dy*dy < 32 * 32.0f)
    {
      tar = EntityGet(&w->manager, id);
      TraceLog(LOG_INFO, "Found target! Entity %u -> %u", e.id, id);
      break;
    }
  }

  if(EntityValid(&w->manager, tar)){
    relation_t* r = EntityAddRelation(w, e, REL_TargetOf, tar);
    if(r)
      return BEHAVIOR_SUCCESS;
  }

  return BEHAVIOR_FAILURE;
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
