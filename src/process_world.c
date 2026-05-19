#include "process_event.h"
#include "scene_loader.h"

void InitGameWorld(void){
  WorldInit(&world);
  InitGameDefine(&world);
}

void GameLoad(void){
  Scene* test = GameCalloc("GameLoad", 1, sizeof(Scene));

  bool scene = SceneLoadByIndex(0, test);

  SceneSetup(&world, test);

}
