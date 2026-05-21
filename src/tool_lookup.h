#ifndef __TOOL_LOOK__
#define __TOOL_LOOK__

#include "physics_enum.h"

CameraTracking StringToCameraMode(const char* str);
ActionType StringToAction(const char* str);
Vector2 StringToVector2(const char* str);
ForceType StringToForce(const char* str);
ShapeType StringToShape(char* str);
static const char* RENDER_LAYER_LOOK[LAYER_DONE] = {
  "LAYER_BACKGROUND",
  "LAYER_FLOOR",
  "LAYER_MAIN",
  "LAYER_TOP",
  "LAYER_UI",
};

static const char* LookupLayer(int l){
  return RENDER_LAYER_LOOK[l];

}
#endif

