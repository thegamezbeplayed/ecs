#include "game_assets.h"
#include "game_strings.h"
//#include "game_math.h"
#include "game_tools.h"
//#include "screens.h"
#include "character_asset.h"

void InitResources(){
  TEXTURES[SHEET_CHAR] = CHAR_SPRITES;
  Image spritesImg = LoadImage(TextFormat("resources/%s",CHAR_IMAGE_PATH)); 
  SHEETS[SHEET_CHAR].sprite_sheet = GameMalloc("",sizeof(Texture2D));
  SpriteLoadSubTextures(CHAR_SPRITES,SHEET_CHAR, NUM_CHAR);
  *SHEETS[SHEET_CHAR].sprite_sheet = LoadTextureFromImage(spritesImg);

}

sprite_t* InitSpriteByTag(char *name, SheetID s){
  sprite_t* spr = GameCalloc("InitSprite", 1, sizeof(sprite_t));
  sprite_sheet_data_t* data = &SHEETS[s];

  uint32_t hash = hash_str_32(name);
  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->tag != hash)
      continue;

    spr->slice = data->sprites[i];
    spr->sheet = data->sprite_sheet;

    spr->is_visible = true;
    spr->offset = spr->slice->offset;
    return spr;
  }

  return NULL;

}

void AnimAddGroup(anim_player_t* a, sprite_slice_t* s, int index, int count){
  anim_t* anim = &a->anims[index];

  sprite_slice_t* as = &anim->frames[anim->num_frames++];
  memcpy(as, s, sizeof(sprite_slice_t));
}

anim_player_t* InitAnimGroup(char* tag, SheetID s){
  anim_player_t* ap = GameCalloc("InitAnimGroup", 1, sizeof(anim_player_t));
  sprite_sheet_data_t* data = &SHEETS[s];
  uint32_t hash = hash_str_32(tag);

  sprite_slice_t *found[128];

  int num_groups = 0;
  int count = 0;
  int largest = 0;
  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->tag != hash)
      continue;

    if(data->sprites[i]->group == 0)
      continue;

    if(data->sprites[i]->index > largest)
      largest = data->sprites[i]->index;

    if(data->sprites[i]->index == 0){
      ap->groups |= data->sprites[i]->group;
      num_groups++;

    }

    found[count++] = data->sprites[i];
  }

  if(num_groups == 0)
    return NULL;

  ap->anims = GameCalloc("InitAnimGroup", num_groups, sizeof(anim_t));
  for (int i = 0; i < num_groups; i++)
    ap->anims[i].frames = GameCalloc("AnimAddGroup", count, sizeof(sprite_slice_t));

  largest+=1;
  int index = -1;
  for (int i = 0; i < count; i++){ 
    if(found[i]->index == 0)
      index++;

    AnimAddGroup(ap, found[i], index, largest);
  
  }
  ap->num_seq = index;

  return  ap;

}

void DrawSlice(sprite_slice_t *slice, Vector2 position,float rot){
  Rectangle src = slice->bounds;

  position = Vector2Add(position,slice->offset);
  Rectangle dst = {
    position.x,
    position.y,
    slice->bounds.width * slice->scale,
    slice->bounds.height * slice->scale
  };

  Vector2 origin = (Vector2){
    slice->center.x * slice->scale,//offset.x,
      slice->center.y * slice->scale//offset.y
  };

  Texture sheet = *SHEETS[slice->sheet].sprite_sheet;
  Color col = slice->color.a > 0? slice->color: WHITE; 
  DrawTexturePro(sheet,src,dst, origin, rot, col);

  return;
}

bool FreeSprite(sprite_t* s){
  if(!s) return false;

  GameFree("FreeSprite", s);
  return true;
}

void DrawSpriteAtPos(sprite_t*s , Vector2 pos){
  if(s->is_visible)
    DrawSlice(s->slice, s->pos, s->rot);

}

void DrawSprite(sprite_t* s){
  if(!s->slice)
    return;

  if(s->is_visible)
    DrawSlice(s->slice, s->pos, s->rot);
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

    spr->tag = hash_str_32( sub_string(sprData.tag, ",", 0));
    spr->group = hash_str_32( sub_string(sprData.tag, ",", 1));
    spr->index = sprData.index;
    
    spr->center = center;// Vector2Scale(offset,spr->scale);
    spr->offset = VECTOR2_ZERO;//offset;//center;//Vector2Scale(center,spr->scale);
    spr->bounds = bounds;
    SHEETS[id].sprites[SHEETS[id].num_sprites++] = spr;
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
