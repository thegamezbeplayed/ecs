#ifndef __ANIM_DEF__
#define __ANIM_DEF__
#include "game_resources.h"

typedef struct anim_player_s anim_player_t;
typedef struct anim_s anim_t;

struct anim_s{
  char              name[MAX_NAME_LEN];
  int               count, cur_index;
  int               frames[MAX_ANIM_FRAMES];
  int               duration, elapsed;
  float             speed;
  bool              loop, interupt;
  int               hurtbox_index;
  AnimPhase         phase;
  AnimEventID       on_phase[ANIM_STOP];
};

AnimPhase AnimPlay(anim_t*);
anim_t* AnimRegisterState(SheetID, const char* tag, char* group);
struct anim_player_s{
  SheetID         sheet_id;
  AnimState       state;
  int             dir;
  collision_d     hit, hurt;
};

typedef struct{
  anim_player_t   player;
  AnimEventID     event;
  int             num_hurt;
  collision_d     hitbox;
  collision_d     hurtboxes[MAX_SLICES];
  anim_t          sequences[ANIM_END][MAX_DIRECTIONS];
}anim_comp_t;

AnimPhase AnimStart(anim_comp_t*, anim_t*);

bool AnimPlayerState(anim_comp_t*, AnimState s, int dir);
#endif
