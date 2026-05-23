#if defined(PLATFORM_DESKTOP)

#include "app_core.h"
#include "raylib.h"

int main(void)
{
  AppInit((AppConfig){
    .width = 1280,
    .height = 720,
    .fps = 60,
    .title = "raylib game template",
  });

  while (!WindowShouldClose() && !AppWantsQuit()) {
    AppFrame(AppShouldThrottleFrame());
  }

  AppShutdown();
  return 0;
}

#endif
