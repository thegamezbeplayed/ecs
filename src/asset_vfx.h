#ifndef __ASS_PART__
#define __ASS_PART__
#include "game_define.h"
#include "asset_define.h"

#define MAX_PARTICLES        256
#define MAX_PARTICLE_LAYERS  LAYER_DONE

Entity ParticlePoolGetAvailable(RenderLayer l);

DEFINE_EVENT_SPACE(ParticleEvent, EVENT_PARTICLE_BASE);

typedef enum {
  PARTICLE_EVENT_BURST,
  PARTICLE_EVENT_START,
  PARTICLE_EVENT_STOP,
  PARTICLE_EVENT_COUNT
} ParticleEventID;

typedef enum {
  PARTICLE_NONE,
  PARTICLE_SPRITE,
  PARTICLE_RECT,
  PARTICLE_CIRCLE,
  PARTICLE_PIXEL,
} ParticleDrawType;

typedef enum {
  PARTICLE_EMIT_NONE,
  PARTICLE_EMIT_CONTINUOUS,
  PARTICLE_EMIT_BURST,
  PARTICLE_EMIT_EVENT
} ParticleEmitMode;
  
typedef struct {
  bool             active;
  ParticleDrawType draw_type;
  RenderLayer      layer;

  SheetID          sheet_id;
  int              sheet_index;
  Rectangle        source;

  float            wid, hei, radius;

  Vector2          vel, accel;
  int              age, dur;
  float            rot, drag, scale;

  float            start_scale;
  float            end_scale;
  
  float            angular_velocity;

  Color            start_color;
  Color            end_color;
  Color            color;
} particle_t;

typedef struct {
  ParticleEmitMode mode;
  RenderLayer      layer;

  int              max, burst, rate;
  float            emit_accum;
  bool             active;
  bool             local_space;
  
  int              dur_min, dur_max;

  Vector2          offset;
  Vector2          spawn_radius;
  Vector2          velocity_min;
  Vector2          velocity_max;
  Vector2          acceleration;
  
  float            scale_min, scale_max, scale_end;

  float            rotation_min;
  float            rotation_max;
  float            angular_velocity_min;
  float            angular_velocity_max;

  float            wid, hei;  
  Color            start_color;
  Color            end_color;
  
  ParticleDrawType draw_type;
  SheetID          sheet_id;
  int              sheet_index;
} particle_emitter_t;

int ParticleEmit(world_t* w, Entity);
bool ParticleStep(particle_t*, position_t*);
#endif
