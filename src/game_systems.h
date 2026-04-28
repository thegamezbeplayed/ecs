#ifndef __GAME_SYS__
#define __GAME_SYS__
#include "game_register.h"
#include "game_process.h"

typedef struct system_pool_s system_pool_t;

typedef void (*SystemFn)(system_pool_t*, component_registry_t*);

typedef struct{
  int             draw_calls;
  RenderTexture2D tex;
}render_st;

typedef struct{
  int   active_camera;
}camera_st;

typedef struct{
  DataType  type;
}position_st;
void OnPositionEvent(event_t* ev, void* data);

typedef struct{
  bool  step;
  int   turn;
}input_st;
void OnInputEvent(event_t* ev, void* data);

typedef struct{
  float       global_speed;
  hash_map_t  groups;
}animate_st;

typedef struct{
  int       num, cap;
  force_t   *forces;
}phys_st;

typedef struct{
  size_t    size;
  SystemFn  tick[64];
}schedule_step_t;

typedef struct{
  schedule_step_t states[GAME_DONE];
  schedule_step_t steps[UPDATE_DONE];
}scheduler_t;

struct system_pool_s{
  render_st   render;
  camera_st   cam;
  input_st    input;
  position_st pos;
  animate_st  anim;
  phys_st     phys;
  scheduler_t schedule;
};

void InitPhysicsSystem(phys_st* p);
void InitPositionSystem(position_st*);
void InitAnimateSystem(animate_st*);
void RegisterAnimation(animate_st*, uint64_t group);
void InitInputSystem(input_st*);
void InitRenderSystem(render_st*);
void InitCameraSystem(camera_st* c);
void RenderSystem(system_pool_t*, component_registry_t*); 
void RegisterSystems(void);
void SystemsState(GameState);
void SystemsStep(UpdateType);
void RegisterScheduleStep(UpdateType, SystemFn);
void RegisterScheduleState(UpdateType, SystemFn);
#endif
