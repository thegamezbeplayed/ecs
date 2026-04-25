#ifndef __GAME_ASSETS__
#define __GAME_ASSETS__

//#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "game_define.h"
#include "game_types.h"

#define MAX_ANIM_FRAMES 4
#define MAX_SPRITES 128
#define MAX_LAYER_SPRITES 64

#define FLOAT_TEXT_SIZE 54
#define FLOAT_TEXT_SPACING 2

typedef enum{
  CHAR_PLAYER,
  CHAR_DONE
}CharacterSprite;

typedef struct sprite_s sprite_t;
typedef struct sprite_slice_s sprite_slice_t;

typedef struct sub_texture_s {
  int           tag;
  int           originX, originY;
  int           positionX, positionY;
  int           sourceWidth, sourceHeight;
  float         scale;
} sub_texture_t;

void InitResources();

typedef enum{
  SHEET_UI,
  SHEET_ICON,
  SHEET_TILE,
  SHEET_CHAR,
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
  int       id;
  uint64_t  uid;
  SheetID   sheet;
  Vector2   center, offset;
  Rectangle bounds;
  float     scale;
  Color     color;
};

typedef struct{
  int             num_sprites;
  sprite_slice_t  *sprites[128];
  Texture2D       *sprite_sheet;
}sprite_sheet_data_t;

static sub_texture_t* TEXTURES[SHEET_ALL];
static sprite_sheet_data_t SHEETS[SHEET_ALL];
void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap);

void SpriteLoadSlicedTextures();
//SPRITE_T===>
struct sprite_s{
  int               suid;
  Texture2D         *sheet;
  sprite_slice_t*   slice;
  bool              is_visible;
  float             rot;
  Vector2           pos, offset, dest;
  RenderLayer       layer;
};

void DrawSlice(sprite_slice_t*, Vector2 position,float rot);
sprite_t* InitSpriteByID(int id, SheetID);
sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* spritesheet);
bool FreeSprite(sprite_t* s);
void DrawSprite(sprite_t* s);
void DrawSpriteAtPos(sprite_t*s , Vector2 pos);
#endif
