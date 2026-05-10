#ifndef __GAME_DEF__
#define __GAME_DEF__
#include "cJSON.h"

#include "game_register.h"

#define MOB_MAX 64
#define NUM_SYS 16
#define NUM_REL 4
#define ROOM_SIZE (Vector2){1600,1200}

DEFINE_EVENT_SPACE(CombatEvent, EVENT_COMBAT_BASE);

extern int PHYS_SYS;

typedef struct{
  const char*  name;
  int          num_comp;
  const char*  components[MAX_COMPONENTS];
}prefab_entity_t;

typedef struct{
  const char*   name;
  RelationType  type;
}relation_pair_t;

typedef struct{
  const char*     comp;
  int             count;
  relation_pair_t pairs[MAX_RELATIONS_PER_ENTITY];
}component_relation_t;

typedef struct{
    const char*           name;
    int                   count;
    component_relation_t  comps[MAX_RELATIONS];
}entity_relation_t;

typedef struct{
  int                 num_comps;
  const char*         comps[NUM_COMP_CORE];
  int                 num_prefabs;
  prefab_entity_t*    prefabs;
  int                 relation_count;
  entity_relation_t*  relations;
}game_t;
bool ParseGameDefinition(cJSON* root, game_t* out);
void LoadGameDefine(const char* path);

extern const component_define_t CORE_COMPONENTS[NUM_COMP_CORE];

typedef struct{
  RelationType    type;
  const char      name[MAX_NAME_LEN];
}relation_str_t;

static const relation_str_t RELATION_LOOKUP[NUM_REL] = {
  {REL_AppliesTo,   "AppliesTo"},
  {REL_ChildOf,     "ChildOf"},
  {REL_Owner,       "Owner"},
  {REL_Target,      "Target"}
};

static int FindComponentIndex(const char* name)
{
    for (int i = 0; i < NUM_COMP_CORE; i++) {
        if (strcmp(CORE_COMPONENTS[i].name, name) == 0)
            return i;
    }
    return -1;
}
#endif
