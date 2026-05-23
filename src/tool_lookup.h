#ifndef __TOOL_LOOK__
#define __TOOL_LOOK__

#include "physics_enum.h"
#include "sprite_enum.h"
#include "vfx_enum.h"
#include "view_enum.h"
#include "process_enum.h"
#include "behavior_define.h"

State StringToState(const char* str);
BehaviorLeafInit StringToLeafFunc(const char* str);
AnimBehavior StringToAnimBehavior(const char* str);
AnimState StringToAnimState(const char* str);
ParticleEmitMode StringToEmitMode(const char* str);
SheetID StringToSheetID(const char* str);
ParticleDrawType StringToDrawType(const char* str);
PhysicsEventID StringToPhysEvent(const char* str);
ReactType StringToReaction(const char* str);
CameraTracking StringToCameraMode(const char* str);
ActionType StringToAction(const char* str);
Vector2 StringToVector2(const char* str);
ForceType StringToForce(const char* str);

CollType StringToCollType(const char* str);
ShapeType StringToShape(const char* str);
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

UpdateType GetUpdateStep(const char* name);
GameState GetGameState(const char* name);
SystemFn SystemFunctionLookup(const char* name);
ComponentInitFn ComponentFuncLookup(const char* name);
RelationType RelationTypeLookup(char* str);
#endif

