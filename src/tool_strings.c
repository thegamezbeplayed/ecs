#include "tool_lookup.h"
#include "game_math.h"
#include "game_strings.h"
#include "system_define.h"
#include "game_define.h"

#include <string.h>

State StringToState(const char* str){
  if (strcmp(str, "IDLE") == 0)   return STATE_IDLE;
  if (strcmp(str, "SPAWN") == 0)  return STATE_SPAWN;
  if (strcmp(str, "AGGRO") == 0)  return STATE_AGGRO;
  if (strcmp(str, "ATTACK") == 0) return STATE_ATTACK;
  if (strcmp(str, "DIE") == 0)    return STATE_DIE;
 
  return STATE_NONE;
}

BehaviorTreeType StringToBehaviorType(const char* str){
  if (strcmp(str, "LEAF") == 0)       return BT_LEAF;
  if (strcmp(str, "SEQUENCE") == 0)   return BT_SEQUENCE;
  if (strcmp(str, "SELECTOR") == 0)   return BT_SELECTOR;
  if (strcmp(str, "CONCURRENT") == 0) return BT_CONCURRENT;
  if (strcmp(str, "DECIDER") == 0)    return BT_DECIDER;
 
  return BT_NONE;
}

BehaviorLeafInit StringToLeafFunc(const char* str){
  if (strcmp(str, "AcquireDestination") == 0)  return LeafAcquireDestination;
  if (strcmp(str, "MoveToDestination") == 0)   return LeafMoveToDestination;
  if (strcmp(str, "MoveToTarget") == 0)        return LeafMoveToTarget;
  if (strcmp(str, "ChangeState") == 0)         return LeafChangeState;
  if (strcmp(str, "CheckAggro") == 0)          return LeafCheckAggro;
  if (strcmp(str, "Attack") == 0)              return LeafAttack;

  return NULL;
}

Vector2 StringToVector2(const char* str){
    if (str == NULL) return VECTOR2_ZERO;

    if (strcmp(str, "MOVE-UP") == 0 || strcmp(str, "UP") == 0) {
        return VECTOR2_UP;
    }
    if (strcmp(str, "MOVE-DOWN") == 0 || strcmp(str, "DOWN") == 0) {
        return VECTOR2_DOWN;
    }
    if (strcmp(str, "MOVE-LEFT") == 0 || strcmp(str, "LEFT") == 0) {
        return VECTOR2_LEFT;
    }
    if (strcmp(str, "MOVE-RIGHT") == 0 || strcmp(str, "RIGHT") == 0) {
        return VECTOR2_RIGHT;
    }

    // Default / unknown
    return VECTOR2_ZERO;
}

ForceType StringToForce(const char* str){
    if (str == NULL) return FORCE_NONE;

    if (strcmp(str, "NONE") == 0)      return FORCE_NONE;
    if (strcmp(str, "GRAVITY") == 0)   return FORCE_GRAVITY;
    if (strcmp(str, "STEERING") == 0)  return FORCE_STEERING;
    if (strcmp(str, "IMPULSE") == 0)   return FORCE_IMPULSE;
    if (strcmp(str, "AVOID") == 0)     return FORCE_AVOID;
    if (strcmp(str, "KINEMATIC") == 0) return FORCE_KINEMATIC;
    if (strcmp(str, "DONE") == 0)      return FORCE_DONE;

    // Unknown string → safe default
    return FORCE_NONE;
}

ReactType StringToReaction(const char* str){
  if (str == NULL) return REACT_NONE;

  if(strcmp(str, "BUMP") == 0) return REACT_BUMP;
  if(strcmp(str, "BLOCK") == 0) return REACT_BLOCK;

  return REACT_NONE;
}

AnimEventID StringToAnimEvent(const char* str){
  if (str == NULL) return ANIM_EVENT_NONE;

  if (strcmp(str, "STATE") == 0)        return ANIM_EVENT_STATE;
  if (strcmp(str, "FRAME") == 0)        return ANIM_EVENT_FRAME;
  if (strcmp(str, "SUSPEND") == 0)      return ANIM_EVENT_SUSPEND;
  if (strcmp(str, "ATTACK") == 0)       return ANIM_EVENT_ATTACK;
  if (strcmp(str, "ATTACK_END") == 0)   return ANIM_EVENT_ATTACK_END;
  if (strcmp(str, "ANIM_END") == 0)     return ANIM_EVENT_SEQ_END;
  if (strcmp(str, "KILL") == 0)         return ANIM_EVENT_KILL;

  return ANIM_EVENT_NONE;
}

PhysicsEventID StringToPhysEvent(const char* str){
  if (str == NULL) return PHYS_EVENT_NONE;

  if (strcmp(str, "ACCEL") == 0)      return PHYS_EVENT_ACCEL;
  if (strcmp(str, "COLL") == 0)       return PHYS_EVENT_COLL;
  if (strcmp(str, "FORCE_END") == 0)  return PHYS_EVENT_FORCE_END;
  if (strcmp(str, "SPAWN") == 0)      return PHYS_EVENT_SPAWN;
  if (strcmp(str, "DESTROY") == 0)    return PHYS_EVENT_DESTROY;
  if (strcmp(str, "HIT") == 0)        return PHYS_EVENT_HIT;

  return PHYS_EVENT_NONE;
}

CollType StringToCollType(const char* str){
  char sub[16];
  strcpy(sub, sub_string(str, "_", 0));
  if(strcmp(sub, "hitbox") == 0)      return COL_HIT;
  if(strcmp(sub, "hurtbox") == 0)   return COL_HURT;

  return COL_NONE;
}

ShapeType StringToShape(const char* str){
 if (strcmp(str, "RECT") == 0) return SHAPE_REC;
 if (strcmp(str, "CIRCLE") == 0) return SHAPE_CIRCLE;

 return SHAPE_NONE;
}

ActionType StringToAction(const char* str){
 if (strcmp(str, "MOVE") == 0) return ACT_MOVE;
 if (strcmp(str, "ATTACK") == 0) return ACT_ATTACK;
 if (strcmp(str, "STOP") == 0) return ACT_STOP;

 return ACT_NONE;
}

CameraTracking StringToCameraMode(const char* str){
  if (strcmp(str, "FOLLOW") == 0) return CAM_FOLLOW;
  if (strcmp(str, "FOLLOW-SMOOTH") == 0) return CAM_FOLLOW_SMOOTH;

  return CAM_NONE;
}

AnimState StringToAnimState(const char* str){
 if (strcmp(str, "IDLE") == 0)   return ANIM_IDLE;
 if (strcmp(str, "WALK") == 0)   return ANIM_WALK;
 if (strcmp(str, "ATTACK") == 0) return ANIM_ATTACK;
 if (strcmp(str, "DIE") == 0)    return ANIM_DIE;
 if (strcmp(str, "HURT") == 0)   return ANIM_HURT;

 return ANIM_NONE;
}

SheetID StringToSheetID(const char* str){
  if (strcmp(str, "SHEET_CHAR") == 0) return SHEET_CHAR;
  if (strcmp(str, "SHEET_PLAYER") == 0) return SHEET_CHAR;
  if (strcmp(str, "SHEET_MOB") == 0)  return SHEET_MOB;
  if (strcmp(str, "SHEET_TILE") == 0) return SHEET_TILE;
  if (strcmp(str, "SHEET_AUTOTILE") == 0) return SHEET_AUTOTILE;
  // ... add others
  return SHEET_ALL;
}

ParticleEmitMode StringToEmitMode(const char* str){
  if(strcmp(str, "CONTINUOUS") == 0) return PARTICLE_EMIT_CONTINUOUS;
  if(strcmp(str, "BURST") == 0) return PARTICLE_EMIT_BURST;
  if(strcmp(str, "EVENT") == 0) return PARTICLE_EMIT_EVENT;

  return PARTICLE_EMIT_NONE;
}

ParticleDrawType StringToDrawType(const char* str){
  if(strcmp(str, "SPRITE") == 0) return PARTICLE_SPRITE;
  if(strcmp(str, "RECT") == 0) return PARTICLE_RECT;
  if(strcmp(str, "CIRCLE") == 0) return PARTICLE_CIRCLE;
  if(strcmp(str, "PIXEL") == 0) return PARTICLE_PIXEL;

  return PARTICLE_NONE;
}

ComponentInitFn ComponentFuncLookup(const char* name){
  if (!name) return NULL;

  for (int i = 0; i < NUM_COMP_CORE; i++)   // your function registry
  {
    if (strcmp(COMPFUNC_LOOKUP[i].name, name) == 0)
      return COMPFUNC_LOOKUP[i].func;   // cast if needed
  }

  TraceLog(LOG_WARNING,"=== COMPONENT FUNC LOOKUP ===\n function '%s' not registered!\n", name);
  return NULL;

}

SystemFn SystemFunctionLookup(const char* name)
{
  if (!name) return NULL;

  for (int i = 0; i < NUM_FUNCTIONS; i++)   // your function registry
  {
    if (strcmp(FUNCTION_LOOKUP[i].name, name) == 0)
      return (SystemFn)FUNCTION_LOOKUP[i].func;   // cast if needed
  }

  TraceLog(LOG_WARNING,"=== SYSTEM LOOKUP ===\n system function '%s' not registered!\n", name);
  return NULL;
}

GameState GetGameState(const char* name){
  if(!name) return -1;

  for(int i = 0; i < GAME_DONE; i++){
    if (strcmp(GAMESTATE_LOOKUP[i].name, name) == 0)
      return GAMESTATE_LOOKUP[i].state;
  }

  TraceLog(LOG_WARNING,"==== GAMESTATE LOOKUP ====\n %s not found!", name);

  return -1;
}

UpdateType GetUpdateStep(const char* name){
  if(!name) return -1;

  for(int i = 0; i < UPDATE_DONE; i++){
    if (strcmp(UPDATE_LOOKUP[i].name, name) == 0)
      return UPDATE_LOOKUP[i].type;
  }

  TraceLog(LOG_WARNING,"==== UPDATE LOOKUP ====\n %s not found!", name);

  return -1;
}

AnimPhase AnimPhaseLookup(const char* str){
  if (!str) return -1;

  for(int i = 0; i < ANIM_STOP; i++){
    if (strcmp(PHASE_LOOKUP[i].name, str) == 0)
      return PHASE_LOOKUP[i].id;
  }

  return -1;

}

notification NotificationFromEventID(const char* str, int id){
  char sub[16];
  strcpy(sub, sub_string(str, "-", 0));
  if(strcmp(sub, "ANIM") == 0)      return AnimEvent_ToNotif(id);
  if(strcmp(sub, "INPUT") == 0)     return InputEvent_ToNotif(id);
  if(strcmp(sub, "POS") == 0)       return PosEvent_ToNotif(id);
  if(strcmp(sub, "BEHAVIOR") == 0)  return BehaviorEvent_ToNotif(id);
  if(strcmp(sub, "PHYS") == 0)      return PhysEvent_ToNotif(id);

  return INVALID_NOTIF;
}

notification EventIDLookup(const char* str){
  for (size_t i = 0; i < EVENT_LOOKUP_COUNT; ++i)
  {
    const event_look_t* e = &EVENT_LOOKUP[i];
    if(strcmp(e->name, str) != 0)
      continue;

    return NotificationFromEventID(str, e->id);
  }

  return -1;
}
