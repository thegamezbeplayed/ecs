#ifndef __COMP_DEF__
#define __COMP_DEF__

#include "game_assets.h"
#include "game_control.h"
#include "game_physics.h"
#include "game_views.h"
#include "game_stats.h"
#include "game_behaviors.h"

typedef struct{
  anim_player_t   player;
  AnimEventID       event;
  int             num_hurt;
  collision_d     hitbox;
  collision_d     hurtboxes[MAX_SLICES];
  anim_t          sequences[ANIM_DONE][MAX_DIRECTIONS];
}anim_comp_t;

typedef struct{
  char  name[MAX_NAME_LEN];
}name_t;

typedef struct{
  int       wid, hei;
  int       num_tiles;
  tile_t*   tiles;
  Rectangle bounds;
}level_t;

typedef struct{
  viewport_t  view;
  Camera2D    camera;
}cam_comp_t;

typedef struct{
  camera_ctx_t      ctx;
  uint32_t          target;
}track_comp_t;

typedef struct{
  bool    assigned;
}follow_comp_t;

typedef struct{

}ai_comp_t;

typedef struct{
  State   state, last;
}state_comp_t;

typedef struct{
  int   duration;
  int   expiration;
}lifetime_t;

static void LifetimeSet(lifetime_t* lf, int dur){
 lf->expiration = WorldGetTime() + dur;
}
#endif
