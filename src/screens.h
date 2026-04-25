#ifndef SCREENS_H
#define SCREENS_H
#include "game_types.h"

#define MAX_CAMERA 4

typedef enum{
  CAM_NONE,
  CAM_FOLLOW,
}CameraTracking;

typedef struct{
  Camera2D        *camera;
  Cell            size,pos,target, offset;
  Rectangle       view,bounds;
}camera_t;
camera_t* InitCamera(float zoom, float rot, Vector2 offset);

typedef struct{
  CameraTracking  mode;
}camera_ctx_t;
camera_ctx_t* InitCameraContext(CameraTracking mode);

void ScreenCameraSync(camera_t*, Vector2);
void ScreenCameraToggle(void);
bool ScreenCameraSetView(Cell v);
void ScreenCameraSetBounds(Cell b);
Rectangle ScreenGetCameraView(void);
void ScreenRender(Color);

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
