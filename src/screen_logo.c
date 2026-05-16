#include "screens.h"
#include "app_resource.h"
#include "game_process.h"

int count = 0;
// Logo Screen Initialization logic
void InitLogoScreen(void)
{
  GameSetState(GAME_LOADING);
}

// Logo Screen Update logic
void UpdateLogoScreen(void)
{
  count++;
#if defined(PLATFORM_WEB)
  if(count > 180)
    GameSetState(GAME_FINISHED);
#endif
}

// Logo Screen Draw logic
void DrawLogoScreen(void)
{
  BeginDrawing();
  SpriteRenderSplash();
  EndDrawing();
}

// Logo Screen Unload logic
void UnloadLogoScreen(void)
{
  // TODO: Unload TITLE screen variables here!
  //MenuSetState(&ui.menus[MENU_MAIN],MENU_INACTIVE);
}
