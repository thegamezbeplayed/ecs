#include "raylib.h"
#include "game_tools.h"
#include "game_process.h"
#if defined(PLATFORM_ANDROID)
#include <jni.h>
#endif

camera_ctx_t* InitCameraContext(CameraTracking mode){
  camera_ctx_t* c = GameCalloc("InitCamera", 1,sizeof(camera_ctx_t));

  c->mode = mode;

  return c;
}

camera_t* InitCamera(float zoom, float rot, Vector2 offset){
  camera_t* cam = GameCalloc("InitCamera", 1,sizeof(camera_t));
  Camera2D* raycam = GameCalloc("InitCamera", 1,sizeof(Camera2D));

  raycam->offset = offset;
  raycam->rotation = rot;
  raycam->zoom = zoom;

  raycam->target = VECTOR2_ZERO;

  cam->bounds = Rect(0, 0, offset.x*2, offset.y*2);
  cam->view = Rect(0, 0, offset.x*2, -2*offset.y);
  cam->target = CELL_UNSET;
  cam->camera = raycam;

  return cam;
}

void ScreenCameraToggle(void){
  /*
   * cam->mode = !cam->mode;

  if(cam->mode){
    BeginTextureMode(cam->render);
    ClearBackground(BLACK);
    BeginMode2D(*cam->camera);

  }
  else{
    EndMode2D();
    EndTextureMode();
    DrawTexturePro(cam->render.texture, cam->view, cam->bounds, VECTOR2_ZERO, 0, WHITE);
  }
  */
}

void ScreenCameraSync(camera_t* cam, Vector2 target){
  cam->camera->target = target;
}
