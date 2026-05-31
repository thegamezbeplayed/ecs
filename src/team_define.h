#ifndef __TEAM_DEF__
#define __TEAM_DEF__
#include "game_common.h"
#include "util_hash.h"
#define EVENT_TEAM_BASE   0xC000

DEFINE_EVENT_SPACE(TeamEvent, EVENT_TEAM_BASE);

typedef enum{
  TEAM_EVENT_NONE,
  TEAM_EVENT_CHARM,
  TEAM_EVENT_COUNT
}TeamEventID;

typedef struct {
  char        name[MAX_NAME_LEN];
  uint64_t    id;
  uint64_t    target_id;
  TeamEventID event;
}team_t;

typedef struct{
  char        name[MAX_NAME_LEN];
  hash_map_t  members;
}team_pool_t;

extern hash_map_t TEAMS;

void TeamPoolRegister(const char* name);
void TeamMapInit(int cap);
void TeamRegisterMember(const char* name, Entity e);
#endif
