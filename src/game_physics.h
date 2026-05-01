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
  PHYS_EVENT_ACCEL,
  PHYS_EVENT_COUNT
}PhysicsEventID;

typedef enum {
  FORCE_GRAVITY,
  FORCE_STEERING,
  FORCE_IMPULSE,
  FORCE_AVOID,
  FORCE_KINEMATIC,
  FORCE_NONE
}ForceType;

typedef void (*ForceFn)(rigid_body_t *a, rigid_body_t *b, force_t*);
typedef void (*ForceCb)(rigid_body_t *a, force_t*);
void ForceReactBump(rigid_body_t* a, rigid_body_t* b, force_t*);
void ForceReactBlock(rigid_body_t* a, rigid_body_t* b, force_t*);

struct force_s{
  Vector2    vel, accel, dir, friction;
  ForceType  type;
  float      speed, max_velocity;
  float      threshold;
  ForceFn    on_react;
  ForceCb    on_end;
  bool       is_active;
};

static void ForceEnd(rigid_body_t *b, force_t* f){
  f->is_active = false;
}

void ForceKill(rigid_body_t *b, force_t* f);
force_t* ForceBump(Vector2 acc);
force_t* ForceFromVec2(ForceType type, Vector2 vec);
bool ForceStep(force_t *force, bool accelerate);
void ForceSetDir(rigid_body_t *b, force_t* f, Vector2 dir);
void ForceAddMagnitude(force_t* f, Vector2 mag);
void RigidBodyHasForce(rigid_body_t* b, int cap);
void RigidBodyGiveForce(rigid_body_t* b, force_t* f);
void RigidBodySteer(rigid_body_t* b, Vector2);

typedef struct bounds_s {
  ShapeType   shape;
  Vector2     pos;
  float       radius;
  float       width,height;
} bounds_t;

struct rigid_body_s{
  Vector2     vel;
  int         col_rate;
  force_t     has[FORCE_NONE];
  int         num_forces;
  force_t     apply[MAX_FORCES]; 
  bounds_t    bounds;
  float       restitution;
  bool        is_static, is_grounded;
};

rigid_body_t* InitRigidBody(Vector2 pos, ShapeType, float, float);
int CollisionStep(rigid_body_t*, rigid_body_t*);
static void RigidBodySetPos(rigid_body_t* b, Vector2 pos){
  b->bounds.pos = pos;
}
static void CollisionNoAction(rigid_body_t* a, rigid_body_t* b, force_t*){}
#endif
