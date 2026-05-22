#ifndef __GAME_RES__
#define __GAME_RES__
#include "cJSON.h"
#include "game_common.h"

#define MAX_RESOURCES     4
#define MAX_SPRITES       128
#define MAX_SLICES        8
#define MAX_SPRITE_FRAMES 96
#define MAX_ANIM_GROUPS   20
#define MAX_ANIM_FRAMES   8

cJSON* ParseRoot(const char* path);

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

typedef enum{
  COL_NONE,
  COL_HIT,
  COL_HURT,
}CollType;
  
typedef struct{
  int       frame;
  CollType  type;
  ShapeType shape;
  int       duration, x, y, wid, hei;
}collision_d;

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

typedef struct{
  anim_frame_t frames[MAX_SPRITE_FRAMES];
  int          num_frames;

  anim_tag_t   tags[MAX_ANIM_GROUPS];
  int          num_tags;
  slice_d      slices[MAX_ANIM_FRAMES];
  int          num_slices;
}ase_sprite_sheet_d;
bool LoadAsepriteSheet(cJSON*, ase_sprite_sheet_d*);

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


void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap);
void SpriteLoadSlicedTextures();

collision_d* InitSpriteCollision(anim_frame_t*, CollType, ShapeType, Rectangle r);

//void AsepriteToSprite(SheetID, const ase_sprite_sheet_d*, anim_tag_t, int, sprite_d*); 
#endif
