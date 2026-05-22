#include "game_math.h"
#include "physics_enum.h"
#include "view_enum.h"

#include <string.h>

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
