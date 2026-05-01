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

#define FLOAT_TEXT_SIZE 54
#define FLOAT_TEXT_SPACING 2

typedef struct sprite_s sprite_t;
typedef struct sprite_slice_s sprite_slice_t;

typedef struct{
  ShapeType shape;
  int       posx, posy, wid, hei;
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

void InitResources();

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
  LAYER_ROOT = -1,
  LAYER_BG,
  LAYER_MAIN,
  LAYER_TOP,
  LAYER_DONE
}RenderLayer;

struct sprite_slice_s{
  char      name[MAX_NAME_LEN];
  uint32_t  hash, tag;
  uint64_t  group;
  int       index, angle;
  SheetID   sheet;
  Vector2   center, offset;
  Rectangle bounds;
  float     scale;
  Color     color;
};

typedef struct{
  int             sheet_index;
  SheetID         sheet_id;
  uint32_t        tag;
  int             duration;
  sprite_slice_t  slice;
  collision_d     coll;
}sprite_d;

typedef struct{
  int        num_sprites;
  sprite_d   sprites[128];
  Texture2D  texture;
}sprite_sheet_d;

static sub_texture_t* TEXTURES[SHEET_ALL];
extern sprite_sheet_d SHEETS[SHEET_ALL];
void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap);
sprite_sheet_d LoadSpriteSheet(SheetID, const char*, const char* );
typedef struct anim_player_s anim_player_t;
typedef struct anim_s anim_t;
typedef bool (*AnimCallback)(anim_player_t*, anim_t*);

typedef enum{
  ANIM_IDLE,
  ANIM_WALK,
  ANIM_DIE,
  ANIM_DONE
}AnimState;

struct anim_s{
  char              name[MAX_NAME_LEN];
  int               count, cur_index;
  int               frames[MAX_ANIM_FRAMES];
  int               duration, elapsed;
  float             speed;
  bool              loop;
  AnimCallback      on_end;
};

bool AnimPlay(anim_t*);
anim_t* AnimRegisterState(SheetID, char* tag, char* group);
struct anim_player_s{
  SheetID         sheet_id;
  AnimState       state;
  int             dir;
  collision_d     col_data;
};

bool AnimIdle(anim_player_t*, anim_t*);

void SpriteLoadSlicedTextures();
//SPRITE_T===>
struct sprite_s{
  int               sheet_id, index;
  float             rot;
  Vector2           offset;
};

sprite_t* InitSprite(SheetID, int);

void DrawSlice(sprite_slice_t*, Vector2 position,float rot);
#endif
