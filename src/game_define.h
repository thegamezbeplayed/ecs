#pragma once
#include <stdint.h>
#include "game_assets.h"
#include "game_control.h"
#include "game_physics.h"
#include "game_views.h"

#define MOB_MAX 64
#define NUM_SYS 4

#define ROOM_SIZE (Vector2){1600,1200}

extern uint64_t ANIM_ID;
extern uint64_t NAME_ID;
extern uint64_t POS_ID;
extern uint64_t INPUT_ID;
extern uint64_t PHYS_ID;
extern uint64_t LVL_ID;
extern uint64_t CAM_ID;
extern uint64_t TRACK_ID;
extern uint64_t SPR_ID;
extern uint64_t TYPE_ID;
extern uint64_t FOLLOW_ID;

extern int PHYS_SYS;

typedef struct{
  anim_player_t player;
  anim_t        sequences[ANIM_DONE][MAX_DIRECTIONS];
}anim_comp_t;

typedef struct{
  sprite_t    sprite;
  collision_d coll;
  RenderLayer layer;
}spr_comp_t;

typedef struct{
  char  name[MAX_NAME_LEN];
}name_comp_t;

typedef struct{
  position_t   pos;
}pos_comp_t;

typedef struct{
  Entity    ent;
  input_t   input;
}input_comp_t;

typedef struct{
  rigid_body_t rb;
}phys_comp_t;

typedef struct{
  int       wid, hei;
  int       num_tiles;
  tile_t*   tiles;
  Rectangle bounds;
}lvl_comp_t;

typedef struct{
  viewport_t  view;
  Camera2D    camera;
}cam_comp_t;

typedef struct{
  camera_ctx_t      ctx;
  uint32_t          target;
}track_comp_t;

typedef struct{
  Entity  e;
  bool    assigned;
}follow_comp_t;

typedef struct{
  EntityType type;
}type_comp_t;
