#include "screens.h"
#include "app_resource.h"

// Logo Screen Initialization logic
void InitLogoScreen(void)
{

}

// Logo Screen Update logic
void UpdateLogoScreen(void)
{

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
