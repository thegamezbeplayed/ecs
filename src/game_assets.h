#ifndef __GAME_ASSETS__
#define __GAME_ASSETS__

//#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "game_utils.h"
#include "game_types.h"

#define MAX_ANIM_FRAMES 4
#define MAX_SPRITES 128
#define MAX_LAYER_SPRITES 64

#define FLOAT_TEXT_SIZE 54
#define FLOAT_TEXT_SPACING 2

typedef struct sprite_s sprite_t;
typedef struct sprite_slice_s sprite_slice_t;

typedef struct sub_texture_s {
  const char*   name;
  const char*   tag;
  int           index;
  int           originX, originY;
  int           positionX, positionY;
  int           sourceWidth, sourceHeight;
  int           dur;
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
  uint32_t  hash, tag;
  uint64_t  group;
  int       index, dur;
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

typedef struct anim_player_s anim_player_t;
typedef bool (*AnimCallback)(anim_player_t*);

typedef enum{
  ANIM_NONE,
  ANIM_PLAY,
  ANIM_DONE
}AnimState;

typedef struct{
  uint64_t          group;
  int               cap, seq, num_frames, cur_index;
  sprite_slice_t    *frames;
  int               duration;
  int               elapsed;
  float             speed;
  bool              loop;
  AnimState         state;
  AnimCallback      on_end;
}anim_t;

struct anim_player_s{
  int         num_seq, cur_seq, next;
  uint32_t    groups;
  anim_t      *anims;
  hash_map_t  map;
};

anim_t* AnimGet(anim_player_t* a, hash_key_t key);
void AnimSetSequence(anim_player_t* ap, anim_t* a);
void AnimEvent(event_t*, void*);
sprite_slice_t* AnimGetFrame(anim_player_t* player);
void AnimPlay(anim_player_t*, float);
void AnimEvent(event_t* ev, void* data);
void SpriteLoadSlicedTextures();
//SPRITE_T===>
struct sprite_s{
  int               suid;
  Texture2D         *sheet;
  sprite_slice_t*   slice;
  bool              is_visible;
  float             rot;
  Vector2           offset, dest;
  RenderLayer       layer;
};

void DrawSlice(sprite_slice_t*, Vector2 position,float rot);
sprite_t* InitSpriteByTag(char *name, SheetID);
sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* spritesheet);
anim_player_t* InitAnimGroup(char*, SheetID);
bool FreeSprite(sprite_t* s);
void DrawSprite(sprite_t* s);
void DrawSpriteAtPos(sprite_t*s , Vector2 pos);
#endif
