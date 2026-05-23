#include "app_core.h"

#include <stdlib.h>
#include <time.h>

#include "app_resource.h"
#include "asset_sfx.h"
#include "game_ui.h"
#include "observer_types.h"
#include "platform_loader.h"
#include "process_define.h"
#include "scene_loader.h"
#include "screens.h"
#include "util_observe.h"

LoadQueue loader = {0};

static AppConfig appConfig = {0};
static bool windowReady = false;
static bool wantQuit = false;
static bool resLoaded = false;
static float loadProgress = 0.0f;
static double currentTime = 0.0;
static double updateDrawTime = 0.0;
static double previousTime = 0.0;
static double waitTime = 0.0;

static void AppUpdateLoading(void)
{
  if (resLoaded)
    return;

  loadProgress = PlatformLoaderProgress(&loader);
  if (PlatformLoaderFinished(&loader)) {
    resLoaded = true;
    GameTransitionScreen();
  }
}

void AppInit(AppConfig config)
{
  appConfig = config;
  GameSetFrameRate(appConfig.fps);
  srand((unsigned int)time(NULL));

  InitScreenWindow((float)appConfig.width, (float)appConfig.height, appConfig.title);
  windowReady = true;
  AppOnResize(appConfig.width, appConfig.height);

  GameSetScreen(SCREEN_LOGO);
  InitAudioDevice();

  SpriteLoadSplash("resources/splash.png", VEC_NEW(appConfig.width, appConfig.height));
  InitAudio();
  InitGameProcess();

  SceneInit(&loader);
  PlatformLoaderStart(&loader);

  InitResources();
  InitUI();
  AppUpdateLoading();
}

void AppFrame(bool wait)
{
  AppUpdateLoading();
  GameProcessSync(wait);
}

void AppShutdown(void)
{
  CloseAudioDevice();
  CloseWindow();
}

void AppOnResize(int width, int height)
{
  if (width <= 0 || height <= 0)
    return;

  appConfig.width = width;
  appConfig.height = height;

  if (windowReady) {
    SetWindowSize(width, height);
    window_resize_t resize = { width, height };
    SubjectNotify("WINDOW", &resize);
  }
}

bool AppWantsQuit(void)
{
  return wantQuit;
}

bool AppShouldThrottleFrame(void)
{
  if (appConfig.fps <= 0) {
    previousTime = GetTime();
    return false;
  }

  currentTime = GetTime();
  updateDrawTime = currentTime - previousTime;
  waitTime += updateDrawTime;

  if (waitTime < 1.0f / appConfig.fps) {
    previousTime = currentTime;
    return true;
  }

  waitTime = 0.0;
  previousTime = currentTime;
  return false;
}

float AppGetLoadingProgress(void)
{
  return loadProgress;
}

bool AppIsLoaded(void)
{
  return resLoaded;
}
