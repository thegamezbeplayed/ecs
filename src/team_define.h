#ifndef __TEAM_DEF__
#define __TEAM_DEF__
#include "game_common.h"

typedef struct {
  char      name[MAX_NAME_LEN];
  uint64_t  id;
}team_t;

typedef struct{
  char        name[MAX_NAME_LEN];
  hash_map_t  members;
}team_pool_t;

extern hash_map_t TEAMS;

void TeamRegister(const char* name);
void TeamMapInit(int cap);
void TeamRegisterMember(const char* name, Entity e);
#endif
