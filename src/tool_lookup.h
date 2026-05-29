#ifndef __TOOL_LOOK__
#define __TOOL_LOOK__
#include "physics_enum.h"
#include "sprite_enum.h"
#include "vfx_enum.h"
#include "view_enum.h"
#include "process_enum.h"
#include "behavior_define.h"
#include "game_define.h"

#define EVENT_LOOKUP_COUNT   (sizeof(EVENT_LOOKUP) / sizeof(EVENT_LOOKUP[0]))

State StringToState(const char* str);
BehaviorTreeType StringToBehaviorType(const char* str);
BehaviorLeafInit StringToLeafFunc(const char* str);
AnimState StringToAnimState(const char* str);
ParticleEmitMode StringToEmitMode(const char* str);
SheetID StringToSheetID(const char* str);
ParticleDrawType StringToDrawType(const char* str);
PhysicsEventID StringToPhysEvent(const char* str);
AnimEventID StringToAnimEvent(const char* str);
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

typedef struct{
  int         id;
  const char  name[MAX_NAME_LEN];
}event_look_t;

static const event_look_t EVENT_LOOKUP[] = {
  {ANIM_EVENT_STATE,      "ANIM-STATE"},
  {ANIM_EVENT_SEQ_END,    "ANIM-END"},
  {BEHAVIOR_EVENT_STATE,  "BEHAVIOR-STATE"},
  {INPUT_EVENT_MOVE,      "INPUT-MOVE"},
  {PHYS_EVENT_VEL,        "PHYS-VEL"},
  {PHYS_EVENT_COLL,       "PHYS-COL"},
  {PHYS_EVENT_HIT,        "PHYS-HIT"},
  {PHYS_EVENT_VEL,        "PHYS-VEL"},
  {POS_EVENT_STEP,        "POS-STEP"},
};

notification EventIDLookup(const char* str);

typedef struct{
  State       state;
  const char  str[MAX_NAME_LEN];
}state_look_t;

static const state_look_t STATE_LOOKUP[STATE_END] = {
  {STATE_NONE,    "NONE"},
  {STATE_SPAWN,   "SPAWN"},
  {STATE_IDLE,    "IDLE"},
  {STATE_AGGRO,   "AGGRO"},
  {STATE_ATTACK,  "ATTACK"},
  {STATE_DIE,     "DIE"},
};

typedef struct{
  AnimPhase   id;
  const char  name[MAX_NAME_LEN];
}phase_look_t;

static const phase_look_t PHASE_LOOKUP[ANIM_STOP] = {
  {ANIM_START,    "START"},
  {ANIM_STEP,     "STEP"},
  {ANIM_DONE,     "DONE"},
  {ANIM_ACTION,   "ACTION"},
  {ANIM_RUNNING,  "RUNNING"},
};

AnimPhase AnimPhaseLookup(const char* str);
#endif

