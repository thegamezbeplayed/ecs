#ifndef __GAME_DEF__
#define __GAME_DEF__
#include "cJSON.h"

#include "game_register.h"

#define NUM_SYS      16
#define MAX_COMP_DEF 128
#define MAX_PREF_DEF 64
#define NUM_REL      24

typedef struct tile_instance_t {
  char* name;
  int   tile_index;      // from tileset
  int   prefab_index;    // optional
  int   start_x, start_y;
  int   cell_x, cell_y;
  int   rotation;
  bool  flip_x;
  bool  flip_y;
} tile_instance_t;

typedef struct entity_instance_t {
  char*       prefab;           // "player", "slime", etc.
  int         type;             // ENT_PLAYER, ENT_MOB, ...
  float       x, y;
}entity_instance_t;

typedef struct{
  char*  name;
  int    num_comp;
  char*  components[MAX_COMPONENTS];
}prefab_entity_t;

typedef struct{
  char*         name;
  RelationType  type;
  int           num_comp;
  char*         components[MAX_COMPONENTS];
}relation_pair_t;

typedef struct{
  char*           comp;
  int             count;
  relation_pair_t pairs[MAX_RELATIONS_PER_ENTITY];
}component_relation_t;

typedef struct{
  char*                 name;
  int                   count;
  component_relation_t  comps[MAX_RELATIONS];
}entity_relation_t;

typedef struct component_entry_s component_entry_t;
typedef bool (*ComponentInitFn)(void*, component_entry_t*);

struct component_entry_s{
  char*           comp;
  char*           name;
  int             index;
  ComponentInitFn func;
  cJSON*          data;
};

typedef struct{
  const char      name[MAX_NAME_LEN];
  ComponentInitFn func;
}component_func_t;
ComponentInitFn ComponentFuncLookup(const char* name);
extern const component_func_t COMPFUNC_LOOKUP[];

typedef struct{
  char*       name;
  SystemFn    states[GAME_DONE];
  SystemFn    steps[UPDATE_DONE];
  SystemCB    syncs[UPDATE_DONE];
  SystemCB    sets[GAME_DONE];
  int         num_req;
  const char* components[NUM_REL];
  bool        iter;
}system_define_t;

typedef struct{
  int                 num_sys;
  system_define_t     systems[NUM_SYS];
  component_entry_t   comp_defs[MAX_COMP_DEF];
  int                 num_prefabs;
  prefab_entity_t     prefabs[MAX_PREF_DEF];
  int                 relation_count;
  entity_relation_t*  relations;
}game_t;
bool InitGameDefine(world_t*);
bool ParseSystems(cJSON* root, game_t* out);
bool ParseComponents(cJSON* root, game_t* out);
bool ParsePrefabs(cJSON* root, game_t* out);
bool ParseRelations(cJSON* root, game_t* out);
bool LoadGameDefine(game_t*);
void GameInitPrefabs(world_t* w, game_t* g);
void UnloadGameDefine(game_t* g);
void GameSpawn(world_t* w, game_t* g);

extern const component_define_t CORE_COMPONENTS[];
static int FindComponentIndex(const char* name)
{
  for (int i = 0; i < NUM_COMP_CORE; i++) {
    if (strcmp(CORE_COMPONENTS[i].name, name) == 0)
      return i;
  }
  return -1;
}

typedef struct{
  UpdateType  type;
  const char  name[MAX_NAME_LEN];
}update_define_t;

typedef struct{
  GameState   state;
  const char  name[MAX_NAME_LEN];
}game_state_define_t;

#endif
