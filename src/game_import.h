#ifndef __GAME_IMP__
#define __GAME_IMP__
//#include "game_views.h"
#include "game_register.h"

#define NUM_COMPONENTS 8

void SceneImport(world_t*, const char*);
typedef struct{
  const char*  name;
  int          num_comp;
  const char*  components[NUM_COMPONENTS];
}EntityPrefab;

typedef struct{
  const char*   name;
  RelationType  type;
}RelationPair;

typedef struct{
  const char*  comp;
  int          count;
  RelationPair pairs[MAX_RELATIONS_PER_ENTITY];
}RelationComp;

typedef struct{
    const char*   name;
    int           count;
    RelationComp  comps[MAX_RELATIONS];
}EntityRelations;
void ImportPrefabRelation(world_t*, Entity, const char*, RelationComp);
void ImportPrefabComponent(world_t*, Entity, const char*, const char*, const char*);

#endif
