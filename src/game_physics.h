#ifndef __GAME_PHYS__
#define __GAME_PHYS__

#include "game_tools.h"

typedef enum {
  FORCE_GRAVITY,
  FORCE_STEERING,
  FORCE_IMPULSE,
  FORCE_AVOID,
  FORCE_KINEMATIC,
  FORCE_NONE
}ForceType;

typedef struct {
  Vector2    vel;
  ForceType  type;
  Vector2    accel;
  float      max_velocity;
  Vector2    friction;
  float      threshold;
  bool       is_active;
}force_t;


typedef struct{
  Vector2     vel, pos;
}rigid_body_t;

#endif
