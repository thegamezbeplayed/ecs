#if defined(PLATFORM_WEB)
#include <time.h>
#include "app_resource.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions
#include "game_ui.h"
#include "asset_sfx.h"
#include "process_define.h"
#include "scene_loader.h"
#include "util_observe.h"

#include "rlgl.h"
#include <emscripten/emscripten.h>

static int screenWidth = 1280;
static int screenHeight = 720;
static bool windowReady = false;

static void NotifyWindowResize(void) {
  window_resize_t resize = { screenWidth, screenHeight };
  SubjectNotify("WINDOW", &resize);
}

// EMSCRIPTEN_KEEPALIVE makes it callable from JS
EMSCRIPTEN_KEEPALIVE
void OnCanvasResize(int width, int height) {
  if (width <= 0 || height <= 0)
    return;

  screenWidth = width;
  screenHeight = height;

  if (windowReady) {
    SetWindowSize(screenWidth, screenHeight);
    NotifyWindowResize();
  }
}

LoadQueue loader = {0};
int tarFPS = 60;
bool resLoaded = false;

void UpdateDrawFrame(void);
int main(void){
  GameSetFrameRate(tarFPS);
  srand((unsigned int)time(NULL));  // seed once using current time

  InitScreenWindow(screenWidth,screenHeight, "raylib game template");
  windowReady = true;
  SetWindowSize(screenWidth, screenHeight);
  NotifyWindowResize();

  GameSetScreen(SCREEN_LOGO);
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

  resLoaded = true;
  GameTransitionScreen();

  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);

  return 0;
}

void UpdateDrawFrame(void){
  GameProcessSync(false);
}

#endif
