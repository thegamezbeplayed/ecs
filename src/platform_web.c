#if defined(PLATFORM_WEB)
#include <time.h>
#include "app_resource.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions
#include "game_ui.h"
#include "asset_sfx.h"
#include "game_process.h"
#include "scene_loader.h"

#include "rlgl.h"
#include <emscripten/emscripten.h>

float screenWidth = 1280.0f;
float screenHeight = 720.0f;
LoadQueue loader = {0};

void UpdateDrawFrame(void);
int main(void){
  srand((unsigned int)time(NULL));  // seed once using current time

  InitScreenWindow(screenWidth,screenHeight, "raylib game template");
  InitAudioDevice();      // Initialize audio device

  SpriteLoadSplash("resources/splash.png", VEC_NEW(screenWidth,screenHeight));
  InitAudio();
  InitGameProcess();
  SceneInit(&loader);

  for(int i = 0; i < loader.count; i++){
    LoadJob* job = &loader.jobs[i];

    *job->dest = LoadTexture(job->path);  
  }
  SceneLoadResources();

  InitResources();
  InitUI();
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
  CloseAudioDevice();     // Close audio context

  CloseWindow();    

  return 0;
}

void UpdateDrawFrame(void){
  GameProcessSync(false);
}

#endif
