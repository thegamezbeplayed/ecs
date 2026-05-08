#ifndef __GAME_ASSETS__
#define __GAME_ASSETS__

//#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "game_utils.h"
#include "game_types.h"
#include "cJSON.h"

#define MAX_SPRITES 128
#define MAX_LAYER_SPRITES 64
#define MAX_SPRITE_FRAMES 80
#define MAX_ANIM_FRAMES 8
#define MAX_ANIM_GROUPS 16
#define MAX_SLICES      8
#define FLOAT_TEXT_SIZE 54
#define FLOAT_TEXT_SPACING 2

DEFINE_EVENT_SPACE(AnimEvent, EVENT_ANIM_BASE)

  typedef enum{
    ANIM_EVENT_NONE,
    ANIM_EVENT_FRAME,
    ANIM_EVENT_FRAME_START,
    ANIM_EVENT_FRAME_END,
    ANIM_EVENT_SEQ_END,
    ANIM_EVENT_COUNT
  }AnimEventID;

typedef struct sprite_s sprite_t;
typedef struct sprite_slice_s sprite_slice_t;

typedef enum{
  COL_NONE,
  COL_HIT,
  COL_HURT,
}CollType;

typedef struct{
  int       frame;
  CollType  type;
  ShapeType shape;
  int       duration, posx, posy, wid, hei;
}collision_d;

typedef struct sub_texture_s {
  const char*   name;
  const char*   tag;
  int           originX, originY;
  int           positionX, positionY;
  int           sourceWidth, sourceHeight;
  collision_d   collider;
  int           dur;
  bool          mirror;
  float         scale;
} sub_texture_t;

typedef struct{
  CollType  type;
  Rectangle bounds;
  int       frame;
}slice_key_t;

typedef struct{
  char        name[MAX_NAME_LEN];
  slice_key_t keys[MAX_SLICES];
  int         num_keys;
}slice_d;

typedef struct{
  uint32_t  hash;
  uint64_t  group;
  char      name[MAX_NAME_LEN];
  char      direction[16];
  bool      repeat;
  int       index_start, index_end;
}anim_tag_t;

typedef struct {
  int       index, duration;
  Rectangle frame_rect;     // position + size in the spritesheet
  Rectangle source_rect;    // spriteSourceSize (trimmed area)
} anim_frame_t;

collision_d* InitSpriteCollision(anim_frame_t*, CollType, ShapeType, Rectangle r);
typedef struct{
  anim_frame_t frames[MAX_SPRITE_FRAMES];
  int          num_frames;

  anim_tag_t   tags[MAX_ANIM_GROUPS];
  int          num_tags;
  slice_d      slices[MAX_ANIM_FRAMES];
  int          num_slices;
}ase_sprite_sheet_d;

Texture2D* LoadAsepriteSheet(const char*, ase_sprite_sheet_d* sheet);
Rectangle GetSlice(const ase_sprite_sheet_d*, const char*, int);
bool IsHitboxActive(const ase_sprite_sheet_d* sheet, int frame);
Rectangle GetHurtbox(const ase_sprite_sheet_d* sheet, int frame);

void InitResources();

typedef enum{
  LAYER_ROOT = -1,
  LAYER_BG,
  LAYER_MAIN,
  LAYER_TOP,
  LAYER_DONE
}RenderLayer;

typedef enum{
  SHEET_UI,
  SHEET_ICON,
  SHEET_TILE,
  SHEET_CHAR,
  SHEET_MOB,
  SHEET_OBJ,
  SHEET_VFX,
  SHEET_ALL
}SheetID;

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
  uint32_t        tag;
  uint64_t        group;
  int             duration;
  sprite_slice_t  slice;
  bool            repeat, mirror;
}sprite_d;

typedef void (*AnimCollisionCB)(sprite_slice_t*, collision_d*, position_t*);

void AsepriteToSprite(SheetID, const ase_sprite_sheet_d*, anim_tag_t, int, sprite_d*); 

void AnimCollisionHurt(sprite_slice_t*,collision_d*, position_t*);

typedef enum{
  ST_ASE,
  ST_RTEXT,
  ST_DONE
}SheetType;

typedef struct{
  int         num_sprites;
  sprite_d    sprites[128];
  int         num_coll;
  collision_d coll[MAX_SLICES];
  Texture2D   texture;
}sprite_sheet_d;

static sub_texture_t* TEXTURES[SHEET_ALL];
extern sprite_sheet_d SHEETS[SHEET_ALL];
void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap);
sprite_sheet_d LoadSpriteSheet(SheetID, const char*, const char* );
void SpriteLoadSlicedTextures();


typedef struct anim_player_s anim_player_t;
typedef struct anim_s anim_t;

typedef enum{
  ANIM_NONE,
  ANIM_START,
  ANIM_IDLE,
  ANIM_WALK,
  ANIM_ATTACK,
  ANIM_HURT,
  ANIM_DIE,
  ANIM_DONE
}AnimState;

typedef enum{
  ANIM_BLANK,
  ANIM_SUSPEND,
  ANIM_HURTBOX,
}AnimBehavior;

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
bool AnimPlayerState(anim_player_t*, anim_t*, AnimState s);
//SPRITE_T===>
struct sprite_s{
  int               sheet_id, index;
  float             rot;
  Vector2           offset;
};
sprite_t* InitSprite(SheetID, int);

void DrawSlice(sprite_slice_t*, Vector2 position,float rot);
#endif
