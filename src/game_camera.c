#include "game_views.h"
#include "game_define.h"

camera_t* InitCamera(float zoom, float rot, Vector2 offset){
  camera_t* c = GameCalloc("InitCamera", 1,sizeof(camera_t));

  c->offset = offset;
  c->rotation = rot;
  c->zoom = zoom;
  c->target = offset;
  c->target = VECTOR2_ZERO;

  return c;
}

viewport_t* InitView(Vector2 size, Rectangle r, float dist){
  viewport_t* v = GameCalloc("InitView", 1,sizeof(viewport_t));

  v->tex = LoadRenderTexture(size.x, size.y);

  v->border = r;
  v->border_distance = dist;
  v->view = Rect(0, 0, size.x, -size.y);
  v->bounds = Rect(0, 0, size.x, size.y);
  return v;
}

camera_ctx_t* InitCameraContext(CameraTracking mode){
  camera_ctx_t* ctx = GameCalloc("InitCameraContext", 1, sizeof(camera_ctx_t));

  ctx->mode = mode;

  ctx->tracking = TRACK_MODES[mode];
  return ctx;
}

void TrackingStatic(camera_t* cam, track_mode_d mode, Vector2 v){
  return;
}

void TrackingFollow(camera_t* cam, track_mode_d t, Vector2 v){
  switch(t.easing){
    case LERP_NONE:
      cam->target = v;
      break;
    case LERP_LINEAR:
      cam->target = Vector2Lerp(cam->target, v, t.speed);
      break;
  }
}
