#include "game_register.h"
#include "game_systems.h"

void AnimEvent(event_t* ev, void* data){
  anim_c* c = &world.anim;

  input_t* in = ev->data;

  anim_player_t* ap = &c->dense[ev->eid];

  uint64_t group = hash_str_64(in->move);
  anim_t* a = AnimGet(ap, group);

  AnimSetSequence(ap, a);
}

uint64_t AnimationGlobal(hash_map_t* m, uint64_t key){
  return HashKey(m, key);
}

void RegisterAnimation(animate_st* a, uint64_t group){
  
  uint64_t key = AnimationGlobal(&a->groups, group);
  if(key > 0)
    return;

  HashPut(&a->groups, group, &group);

  Notification(group, AnimEvent, a);

}

void AnimSystem(system_pool_t* s, component_registry_t* c){
  animate_st* st = &s->anim;
  anim_c* a = &c->anim;

  for(int i = 0; i < a->map.size; i++){
    anim_player_t* ap = &a->dense[i];
    AnimPlay(ap, st->global_speed);
  }
}

void InitAnimateSystem(animate_st* a){
  a->global_speed = 1.f;

  HashInit(&a->groups, next_pow2_int(MAX_COMPONENTS));
  RegisterScheduleStep(UPDATE_FIXED, AnimSystem);

}
