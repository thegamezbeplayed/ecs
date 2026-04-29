#include "game_register.h"
#include "game_systems.h"

void AnimInputEvent(event_t* ev, void* data){
  anim_c* c = &world.anim;

  input_t* in = ev->data;

  anim_player_t* ap = &c->dense[ev->eid];

  uint64_t group = hash_str_64(in->move);
  anim_t* a = AnimGet(ap, group);

  AnimSetSequence(ap, a);
}


void AnimEvent(event_t* ev, void* data){
  position_t* pos = ev->data;

  anim_player_t* ap = data;

  char ang[16];
  int_to_str(pos->angle, ang, sizeof(ang));
  
  uint64_t group = hash_str_64(str_concat("walk", ang));
  anim_t* a = AnimGet(ap, group);

  if(a)
  AnimSetSequence(ap, a);
  else
    TraceLog(LOG_WARNING,"==== ANIM MISSING===\n %s", str_concat("walk",ang));
}

uint64_t AnimationGlobal(hash_map_t* m, uint64_t key){
  return HashKey(m, key);
}

void RegisterAnimation(animate_st* a, uint64_t group){
  
  uint64_t key = AnimationGlobal(&a->groups, group);
  if(key > 0)
    return;

  HashPut(&a->groups, group, &group);


}

void AnimSystem(system_pool_t* s, component_registry_t* c){
  animate_st* st = &s->anim;
  anim_c* a = &c->anim;

  for(int i = 0; i < a->map.size; i++){
    anim_player_t* ap = &a->dense[i];
    AnimPlay(ap, st->global_speed);
  }
}

void LoadAnims(system_pool_t* s, component_registry_t* c){
  animate_st* st = &s->anim;
  anim_c* ac = &c->anim;
  EntityManager *em = &c->manager;
  position_c* pos = &c->pos;
  input_c *in = &c->input;

  for(int i = 0; i < ac->map.size; i++){

    anim_player_t* ap = &ac->dense[i];
    Entity* e = ComponentGetEntity(em, &ac->map, i);


    if(HasComponent(&in->map, *e))
      TargetSubscribe("INPUT_MOVE", AnimInputEvent, ap, e->id );
    else
      TargetSubscribe("DEST_SET", AnimEvent, ap, e->id );


    for (int j = 0; j < ap->num_seq; j++){
      anim_t* a = &ap->anims[j];

      RegisterAnimation(st, a->group);
    }

  }
}

void InitAnimateSystem(animate_st* a){
  a->global_speed = 1.f;

  RegisterScheduleState(GAME_READY, LoadAnims);
  HashInit(&a->groups, next_pow2_int(MAX_COMPONENTS));
  RegisterScheduleStep(UPDATE_FIXED, AnimSystem);

}
