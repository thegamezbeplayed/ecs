#if defined(PLATFORM_DESKTOP)

#include "app_core.h"
#include "raylib.h"

int main(void)
{
  AppInit((AppConfig){
    .width = 1920,
    .height = 1080,
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
