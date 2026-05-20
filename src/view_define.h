#ifndef   __GAME_VIEW__
#define   __GAME_VIEW__

#include "util_tools.h"
#include "view_enum.h"

#define MAX_CAMERA 4

#define TRACK(x, c, v)(x.fn(c, x, v))

typedef Camera2D camera_t;
camera_t* InitCamera(float zoom, float rot, Vector2 offset);

static void CameraSetOffset(camera_t* c, Vector2 offset){
  c->offset = offset;
}

typedef struct{
  Rectangle       view, bounds, border;
  Vector2         origin;
  float           border_distance;
  RenderTexture2D tex;
}viewport_t;
viewport_t* InitView(Vector2 tex_size, Rectangle, float);

typedef struct track_mode_s track_mode_d;

typedef void (*TrackingFn)(camera_t* cam, track_mode_d, Vector2);
void TrackingStatic(camera_t*, track_mode_d, Vector2);
void TrackingFollow(camera_t*, track_mode_d, Vector2);

struct track_mode_s{
  CameraTracking mode;
  InterpoleMode  easing;
  TrackingFn     fn;
  float          speed;
};

typedef struct {
  CameraTracking  mode;
  track_mode_d tracking;
}camera_ctx_t;

camera_ctx_t* InitCameraContext(CameraTracking mode);

void ViewSync(viewport_t*, Vector2);
void CameraToggle(void);
bool CameraSetView(Cell v);
void CameraSetBounds(Cell b);
Rectangle CameraGetView(void);


static track_mode_d TRACK_MODES[CAM_DONE] = {
  {CAM_NONE, LERP_NONE, TrackingStatic},
  {CAM_FOLLOW, LERP_NONE, TrackingFollow},
  {CAM_FOLLOW_SMOOTH, LERP_LINEAR, TrackingFollow, .125f},
};
#endif
