#ifndef __GAME_IMP__
#define __GAME_IMP__
#include "game_register.h"

#define NUM_COMPONENTS 8

void SceneImport(world_t*, const char*);
typedef struct{
  const char*  name;
  int          num_comp;
  const char*  components[NUM_COMPONENTS];
}EntityPrefab;
void ImportPrefabComponent(world_t*, Entity, const char*, const char*, const char*);


void AnimationImport(void*,const char*, const char*);
void SpriteImport(void*,const char*, const char*);
void PhysicsImport(void*,const char*, const char*);
void InputImport(void*,const char*, const char*);
void PositionImport(void*,const char*, const char*);
void CameraImport(void*,const char*, const char*);
void TrackingImport(void*,const char*, const char*);

#endif
