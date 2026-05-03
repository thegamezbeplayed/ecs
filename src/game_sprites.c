#include "game_assets.h"
#include "game_strings.h"
//#include "game_math.h"
#include "game_tools.h"
#include "game_define.h"
#include "game_helpers.h"
//#include "screens.h"
#include "player_atlas.h"
#include "mob_atlas.h"
#include "tile_atlas.h"

sprite_sheet_d SHEETS[SHEET_ALL];

void InitResources(){
  TEXTURES[SHEET_CHAR] = CHAR_SPRITES;
  Image spritesImg = LoadImage(TextFormat("resources/%s",CHAR_IMAGE_PATH)); 
  SpriteLoadSubTextures(CHAR_SPRITES,SHEET_CHAR, NUM_CHAR);
  SHEETS[SHEET_CHAR].texture = LoadTextureFromImage(spritesImg);

  TEXTURES[SHEET_MOB] = MOB_SPRITES;
  Image mobsImg = LoadImage(TextFormat("resources/%s",MOB_IMAGE_PATH)); 
  SpriteLoadSubTextures(MOB_SPRITES,SHEET_MOB, NUM_MOB);
  SHEETS[SHEET_MOB].texture = LoadTextureFromImage(mobsImg);

  SHEETS[SHEET_OBJ] = LoadSpriteSheet(SHEET_OBJ, "resources/fences.json",
      "resources/fences-sheet.png");

/*

  
  TEXTURES[SHEET_TILE] = TILE_SPRITES;
  Image tilesImg = LoadImage(TextFormat("resources/%s",TILE_IMAGE_PATH));
  SHEETS[SHEET_TILE].sprite_sheet = GameMalloc("",sizeof(Texture2D));
  SpriteLoadSubTextures(TILE_SPRITES,SHEET_TILE, NUM_TILE);
  *SHEETS[SHEET_TILE].sprite_sheet = LoadTextureFromImage(tilesImg);
*/
}

sprite_t* InitSprite(SheetID s, int index){
  sprite_t* spr = GameCalloc("InitSprite", 1, sizeof(sprite_t));
  sprite_sheet_d* data = &SHEETS[s];

  spr->sheet_id = s;
  spr->index = index;

  return spr;
}

void DrawSlice(sprite_slice_t *s, Vector2 position,float rot){
  Rectangle src = s->bounds;

  Vector2 origin = (Vector2){
    s->center.x * s->scale,//offset.x,
      s->center.y * s->scale//offset.y
  };

  position = Vector2Add(position,s->offset);
  Rectangle dst = {
    position.x,
    position.y,
    abs(s->bounds.width * s->scale),
    s->bounds.height * s->scale
  };

  Texture sheet = SHEETS[s->sheet].texture;
  Color col = s->color.a > 0? s->color: WHITE; 
  DrawTexturePro(sheet,src,dst, origin, rot, col);

  return;
}

void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap){
  for(int i = 0; i < sheet_cap;i++){
    sub_texture_t sprData = data[i];

    Vector2 center = Vector2FromXY(sprData.originX,sprData.originY);
    Rectangle bounds = RecFromCoords(sprData.positionX,sprData.positionY,sprData.sourceWidth,sprData.sourceHeight);

    sprite_slice_t *spr = GameMalloc("SpriteLoadSubTextures",sizeof(sprite_slice_t));
    memset(spr,0,sizeof(sprite_slice_t));

    spr->sheet = id;
    spr->scale = sprData.scale == 0? 1: sprData.scale;
    spr->hash  = hash_str_32(sprData.name);

    int count;
    char** parts = split_string(sprData.name, '_', &count);
    spr->tag = hash_str_32( sprData.tag);

    strcpy(spr->name, sprData.name); 
    char* group_str = str_concat(parts[1], parts[2]);
    spr->group = hash_str_64(group_str);
    str_to_int(parts[3], &spr->index);
    str_to_int(parts[2], &spr->angle);
    spr->center = center;// Vector2Scale(offset,spr->scale);
    spr->offset = VECTOR2_ZERO;//offset;//center;//Vector2Scale(center,spr->scale);
    spr->bounds = bounds;
 
    if(sprData.mirror){
      spr->center.x = spr->bounds.width - spr->center.x;
      spr->bounds.width*=-1;
    }
   
    sprite_d* sdat = GameCalloc("Sub", 1, sizeof(sprite_d));
    sdat->slice = *spr;
    sdat->coll = sprData.collider;
    sdat->tag = spr->tag;
    sdat->duration = sprData.dur;
    sdat->sheet_index = i;
    sdat->sheet_id = id;
    SHEETS[id].sprites[SHEETS[id].num_sprites++] = *sdat;
  }
}

void SpritePreprocessImg(Image *img, Texture2D *out){
  ImageFormat(img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  // Access pixels
  Color *pixels = LoadImageColors(*img);

  int pixelCount = img->width * img->height;
  for (int i = 0; i < pixelCount; i++)
  {
    // If pixel is white (tolerate near-white)
    if (pixels[i].r > 240 && pixels[i].g > 240 && pixels[i].b > 240)
    {
      pixels[i].a = 0; // Make transparent
    }
  }

  // Apply modified pixels back to image
  Image newImg = {
    .data = pixels,
    .width = img->width,
    .height = img->height,
    .format = img->format,
    .mipmaps = 1
  };

  // Create a texture from modified image
  *out = LoadTextureFromImage(newImg);
  UnloadImageColors(pixels);
}

bool AnimPlay(anim_t* a){
  if(a->elapsed >= a->duration){
    a->cur_index++;
    a->elapsed = 0;
  }
  else{
    a->elapsed++;
    return true;
  }

  if(a->cur_index >= a->count){
    a->cur_index = 0;
    if(a->loop)
      return true;

  }
  else
    return true;

  return false;
}

bool AnimIdle(anim_player_t* player, anim_t* a){
  player->state = ANIM_IDLE;

  return true;
}

anim_t* AnimRegisterState(SheetID id, const char* name, char* group){
  sprite_d found[MAX_SPRITE_FRAMES] = {0};

  int count = 0;
  uint32_t hash = hash_str_32(name);
  uint64_t ghash = hash_str_64(group);
  for (int i = 0; i < SHEETS[id].num_sprites; i++){
    if(SHEETS[id].sprites[i].tag != hash)
      continue;

    sprite_slice_t* spr = &SHEETS[id].sprites[i].slice;

    if(spr->group != ghash)
      continue;

    found[count++] = SHEETS[id].sprites[i];
  } 

  if(count == 0)
    return NULL;

  anim_t* a = GameCalloc("AnimRegisterState", 1, sizeof(anim_t));

  for(int i = 0; i < count; i++){
    a->frames[a->count++] = found[i].sheet_index;

    if(found[i].duration > 0)
      a->duration = found[i].duration;
  }
  strcpy(a->name, group);
  return a;
}
