#ifndef __ASSETS_DEF__
#define __ASSETS_DEF__

//#include "raylib.h"
#include "game_resources.h"
#include "game_utils.h"
#include "game_types.h"
#include "sprite_enum.h"

#define MAX_LAYER_SPRITES  256 
#define FLOAT_TEXT_SIZE    54
#define FLOAT_TEXT_SPACING 2

typedef struct{
  Vector2     offset;
  Color       bg;
  Texture2D   *tex;
}splash_tex_t;

void SpriteLoadSplash(const char* path, Vector2 screen);
void SpriteRenderSplash(void);
static void SpriteUnloadSplash(void){

}

typedef struct sprite_s sprite_t;
typedef struct sprite_slice_s sprite_slice_t;

Rectangle GetSlice(const ase_sprite_sheet_d*, const char*, int);
bool IsHitboxActive(const ase_sprite_sheet_d* sheet, int frame);
Rectangle GetHurtbox(const ase_sprite_sheet_d* sheet, int frame);

void InitResources();

typedef struct{
  char        name[MAX_NAME_LEN];
  RenderLayer layer;
}render_ctx_t;
extern subject_t renderer;

struct sprite_slice_s{
  int       index, angle;
  SheetID   sheet;
  Vector2   center, offset;
  Rectangle bounds;
  float     scale;
  Color     color;
};

typedef struct{
  int             sheet_index;
  char            name[MAX_NAME_LEN];
  SheetID         sheet_id;
  sprite_slice_t  slice;
  uint32_t        tag;
  uint64_t        group;
  int             duration;
  bool            repeat, mirror;
}sprite_d;

typedef struct{
  int         num_sprites;
  sprite_d    sprites[128];
  int         num_coll;
  collision_d coll[MAX_SLICES];
  Texture2D   texture;
}sprite_sheet_d;
static sub_texture_t* TEXTURES[SHEET_ALL];
extern sprite_sheet_d SHEETS[SHEET_ALL];
sprite_sheet_d LoadSpriteSheet(SheetID, const char*, const char* );


typedef void (*AnimCollisionCB)(sprite_slice_t*, collision_d*, position_t*);

void AnimCollisionHurt(sprite_slice_t*,collision_d*, position_t*);

typedef struct anim_player_s anim_player_t;
typedef struct anim_s anim_t;

typedef struct{
  AnimState   state;
  char         name[MAX_NAME_LEN];
  int          dir;
  bool         loop;
  AnimBehavior end, start;
}anim_seq_d;

typedef struct{
  const char    name[MAX_NAME_LEN];
  SheetID       sheet;
  anim_seq_d    sequences[ANIM_DONE][MAX_ANIM_GROUPS];
}anim_d;
bool LoadSceneAnimData(const char*, const char*,  anim_d*);

struct anim_s{
  char              name[MAX_NAME_LEN];
  int               count, cur_index;
  int               frames[MAX_ANIM_FRAMES];
  int               duration, elapsed;
  float             speed;
  bool              loop, interupt;
  int               hurtbox_index;
  AnimState         state;
  AnimBehavior      on_frame_start[MAX_ANIM_FRAMES];
  AnimBehavior      on_end, on_start;
};

AnimEventID AnimPlay(anim_t*);
anim_t* AnimRegisterState(SheetID, const char* tag, char* group);
struct anim_player_s{
  SheetID         sheet_id;
  AnimState       state;
  int             dir;
  collision_d     col_data;
};

bool AnimSetState(anim_t* a, AnimState s);
bool AnimPlayerDirection(anim_player_t* player, int dir);
bool AnimPlayerState(anim_player_t*, anim_t*, AnimState s);
//SPRITE_T===>
struct sprite_s{
  int         sheet_id, index;
  float       rot, scale;
  Vector2     offset;
  RenderLayer layer;
  collision_d coll;
};
sprite_t* InitSprite(SheetID, int);
void DrawSprite(sprite_t* spr, Vector2 position);
sprite_slice_t* InitSliceFromData(sprite_d*);
void DrawSlice(sprite_slice_t*, Vector2 position,float rot);

#endif
