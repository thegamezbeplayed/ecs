#include "team_define.h"
#include "util_tools.h"

hash_map_t TEAMS;

void TeamMapInit(int cap){
  HashInit(&TEAMS, next_pow2_int(cap));
}

void TeamRegister(const char* name){
  hash_key_t key = hash_str_64(name);
  
  team_pool_t* exists = HashGet(&TEAMS, key);
  if(exists)
    return;

  team_pool_t* p = GameCalloc("TeamRegister", 1, sizeof(team_pool_t));

  HashPut(&TEAMS, key, p);
}

void TeamRegisterMember(const char* name, Entity e){
  hash_key_t key = hash_str_64(name);

  team_pool_t* p = HashGet(&TEAMS, key);

  if(!p){
    p = GameCalloc("TeamRegister", 1, sizeof(team_pool_t));
    strcpy(p->name, name);
    HashPut(&TEAMS, key, p);
  }

  HashPut(&p->members, e.id, &e);
}
