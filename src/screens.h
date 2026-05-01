#ifndef SCREENS_H
#define SCREENS_H
//#include "game_types.h"
#include "game_math.h"
#include "game_utils.h"

#define MAX_CAMERA 4

typedef struct{
  bool    is_dragging;
  Vector2 offset,pos;
}mouse_controller_t;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum {
  SCREEN_LOGO,
  SCREEN_TITLE,
  SCREEN_GAMEPLAY,
  SCREEN_ENDING,
  SCREEN_DONE
} GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void BeginDraw(void);
void EndDraw(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);
void PreUpdate(void);
void FixedUpdate(void);
void PostUpdate(void);
//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndScreen(void);
void UpdateEndScreen(void);
void DrawEndScreen(void);
void UnloadEndScreen(void);
int FinishEndScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H
