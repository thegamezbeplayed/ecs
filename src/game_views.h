#ifndef   __GAME_VIEW__
#define   __GAME_VIEW__
#include "game_tools.h"
#include "game_strings.h"

#define MAX_CAMERA 4

#define TRACK(x, c, v)(x.fn(c, x, v))

typedef Camera2D camera_t;
camera_t* InitCamera(float zoom, float rot, Vector2 offset);

typedef struct{
  const char  name[MAX_NAME_LEN];
  float       zoom, rot;
  int         offset_x, offset_y, wid, hei;
  int         bx, by, bw, bh;
}cam_d;

typedef enum{
  CAM_NONE,
  CAM_FOLLOW,
  CAM_FOLLOW_SMOOTH,
  CAM_DONE,
}CameraTracking;

typedef enum{
  LERP_NONE,
  LERP_LINEAR,
  LERP_DONE
}InterpoleMode;

typedef struct{
  Rectangle       view, bounds, border;
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
