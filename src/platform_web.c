#if defined(PLATFORM_WEB)

#include "app_core.h"
#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE
void OnCanvasResize(int width, int height)
{
  AppOnResize(width, height);
}

static void UpdateDrawFrame(void)
{
  AppFrame(false);
}

int main(void)
{
  AppInit((AppConfig){
    .width = 1280,
    .height = 720,
    .fps = 60,
    .title = "raylib game template",
  });

  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
  return 0;
}

#endif
