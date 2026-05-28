#include "asset_define.h"
#include "game_resources.h"
#include "anim_define.h"
#include "component_define.h" 

bool AnimSetPhase(anim_comp_t* ac, anim_t* a, AnimPhase phase){

  return true;
}

AnimPhase AnimStart(anim_comp_t* ac, anim_t* a){
  if(!AnimSetPhase(ac, a, ANIM_START))
    return ANIM_FAIL;

  AnimPhase p = AnimPlay(a);
  if(p < ANIM_DONE)
    return ANIM_START;

  return ANIM_FAIL;
}

AnimPhase AnimPlay(anim_t* a){

  AnimPhase p = a->phase;
  if(a->elapsed >= a->duration){
    p = ANIM_STEP;
    a->cur_index++;
    if(a->hurtbox_index > -1 && a->cur_index == a->hurtbox_index)
      p = ANIM_ACTION;
    a->elapsed = 0;
  }
  else{
    a->elapsed++;
    return ANIM_RUNNING;
  }

  if(a->cur_index >= a->count){
    a->cur_index = 0;
    if(!a->loop)
       p = ANIM_DONE;
  }

  return p;
}

bool AnimPlayerState(anim_comp_t* ac, AnimState s, int dir){
  anim_player_t* player = &ac->player;
  if(s == ANIM_NONE || player->state == s)
    return false;

  anim_t* a = &ac->sequences[player->state][player->dir];
  if(!a->interupt)
    return false;

  if(dir > -1)
    player->dir = dir;

  anim_t* next = &ac->sequences[s][player->dir];
  if(AnimStart(ac, next) == ANIM_START){
    player->state = s;

    player->hurt = ac->hurtboxes[next->hurtbox_index];
    return true;
  }
  
  return false;
}

anim_t* AnimRegisterState(SheetID id, const char* name, char* group){
  sprite_d found[MAX_SPRITE_FRAMES] = {0};

  int count = 0;
  uint32_t hash = hash_str_32(name);
  uint64_t ghash = hash_str_64(group);
  for (int i = 0; i < SHEETS[id].num_sprites; i++){
    if(SHEETS[id].sprites[i].tag != hash)
      continue;

    if(SHEETS[id].sprites[i].group != ghash)
      continue;

    sprite_slice_t* spr = &SHEETS[id].sprites[i].slice;
    found[count++] = SHEETS[id].sprites[i];
  } 

  if(count == 0){
    TraceLog(LOG_WARNING,"=== ANIM REGISTER STATE ===\n Unable to find Tag %s or Group%s", name, group);
    return NULL;
  }
  anim_t* a = GameCalloc("AnimRegisterState", 1, sizeof(anim_t));

  for(int i = 0; i < count; i++){
    a->frames[a->count++] = found[i].sheet_index;
  
    if(found[i].duration > 0)
      a->duration = found[i].duration;
  }

  strcpy(a->name, group);
  return a;
}

bool AnimInit(void* comp, component_entry_t* j){
  anim_comp_t* a = comp;

  return ParseAnimComponent(j->data, a);
}
