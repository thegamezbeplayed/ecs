#ifndef __GAME_PHYS__
#define __GAME_PHYS__

#include "game_tools.h"
#include "game_utils.h"

#define MAX_FORCES 64
#define GRAVITY 0.65f
#define MAX_VELOCITY  16
#define TERMINAL_VELOCITY 7.0f

DEFINE_EVENT_SPACE(PhysEvent, EVENT_PHYS_BASE)

typedef struct rigid_body_s rigid_body_t;
typedef struct force_s force_t;

typedef enum{
  PHYS_EVENT_NONE,
  PHYS_EVENT_ACCEL,
  PHYS_EVENT_COLL,
  PHYS_EVENT_COUNT
}PhysicsEventID;

typedef enum {
  FORCE_NONE,
  FORCE_GRAVITY,
  FORCE_STEERING,
  FORCE_IMPULSE,
  FORCE_AVOID,
  FORCE_KINEMATIC,
  FORCE_DONE
}ForceType;

typedef enum{
  REACT_NONE,
  REACT_BUMP,
  REACT_BLOCK,
}ReactType;

typedef struct{
  const char     name[MAX_NAME_LEN];
  ForceType      type;
  float          speed, max_vel, threshold;
  Vector2        frict;
  PhysicsEventID event;
  ReactType      react;
}force_d;

typedef struct{
  const char  name[MAX_NAME_LEN];
  ShapeType   shape;
  int         wid, hei;
}phys_d;

force_t* ForceReactBump(rigid_body_t* a, rigid_body_t* b, force_t*);
force_t* ForceReactBlock(rigid_body_t* a, rigid_body_t* b, force_t*);

struct force_s{
  Vector2        vel, accel, dir, friction;
  ForceType      type;
  float          speed, max_velocity;
  float          threshold;
  ReactType      react;
  PhysicsEventID event;  
  bool           kill_on_end, is_active;
};

static void ForceEnd(force_t* f){
  f->is_active = false;
}

void ForceApply(rigid_body_t* b, force_t* f);
void ForceKill(force_t* f);
force_t* ForceBump(Vector2 acc);
force_t* ForceFromVec2(ForceType type, Vector2 vec);
bool ForceStep(force_t *force, bool accelerate);
void ForceSetDir(force_t* f, Vector2 dir);
void ForceAddMagnitude(force_t* f, Vector2 mag);

typedef struct bounds_s {
  ShapeType   shape;
  Vector2     pos, offset;
  float       radius;
  float       width,height;
} bounds_t;

struct rigid_body_s{
  Vector2     vel;
  int         col_rate;
  bounds_t    bounds;
  float       restitution;
  bool        is_static, is_grounded;
};

rigid_body_t* InitRigidBody(Vector2 pos, ShapeType, float, float);
static void RigidBodySetPos(rigid_body_t* b, Vector2 pos){
  b->bounds.pos = pos;
}

bool CheckCollision(rigid_body_t *a, rigid_body_t *b, int len);
#endif
