#ifndef __TOOL_LOOK__
#define __TOOL_LOOK__

#include "physics_enum.h"
#include "sprite_enum.h"
#include "vfx_enum.h"
#include "view_enum.h"
#include "process_enum.h"
#include "behavior_define.h"
#include "game_define.h"

typedef struct{
  RelationType    type;
  const char      name[MAX_NAME_LEN];
}relation_str_t;

static const relation_str_t RELATION_LOOKUP[NUM_REL] = {
  {REL_AppliesTo,   "AppliesTo"},
  {REL_ChildOf,     "ChildOf"},
  {REL_Owner,       "Owner"},
  {REL_Target,      "Target"},
  {REL_Observes,    "Observes"},
  {REL_SubjectOf,   "SubjectOf"},
  {REL_BehaviorOf,  "BehaviorOf"}
};

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

static const game_state_define_t GAMESTATE_LOOKUP[GAME_DONE] = {
  {GAME_NONE,  ""},
  {GAME_LOADING,  "GAME_LOADING"},
  {GAME_READY,    "GAME_READY"},
  {GAME_RUNNING,  "GAME_RUNNING"},
  {GAME_PAUSE,    "GAME_PAUSE"},
  {GAME_FINISHED, "GAME_FINISHED"},
  {GAME_OVER,     "GAME_OVER"},
};

static const update_define_t UPDATE_LOOKUP[UPDATE_DONE] = {
  {UPDATE_FRAME,      "UPDATE_FRAME"},
  {UPDATE_DRAW_BEGIN, "UPDATE_DRAW_BEGIN"},
  {UPDATE_DRAW,       "UPDATE_DRAW"},
  {UPDATE_DRAW_END,   "UPDATE_DRAW_END"},
  {UPDATE_START,      "UPDATE_START"},
  {UPDATE_PRE,        "UPDATE_PRE"},
  {UPDATE_FIXED,      "UPDATE_FIXED"},
  {UPDATE_POST,       "UPDATE_POST"},
  {UPDATE_FINAL,      "UPDATE_FINAL"},
};

static const char* LookupLayer(int l){
  return RENDER_LAYER_LOOK[l];

}

UpdateType GetUpdateStep(const char* name);
GameState GetGameState(const char* name);
SystemFn SystemFunctionLookup(const char* name);
ComponentInitFn ComponentFuncLookup(const char* name);
RelationType RelationTypeLookup(const char* str);
#endif

