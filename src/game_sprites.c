#include "game_assets.h"
#include "game_strings.h"
//#include "game_math.h"
#include "game_tools.h"
//#include "screens.h"
#include "player_atlas.h"
#include "mob_atlas.h"

void InitResources(){
  TEXTURES[SHEET_CHAR] = CHAR_SPRITES;
  Image spritesImg = LoadImage(TextFormat("resources/%s",CHAR_IMAGE_PATH)); 
  SHEETS[SHEET_CHAR].sprite_sheet = GameMalloc("",sizeof(Texture2D));
  SpriteLoadSubTextures(CHAR_SPRITES,SHEET_CHAR, NUM_CHAR);
  *SHEETS[SHEET_CHAR].sprite_sheet = LoadTextureFromImage(spritesImg);

  TEXTURES[SHEET_MOB] = MOB_SPRITES;
  Image mobsImg = LoadImage(TextFormat("resources/%s",MOB_IMAGE_PATH)); 
  SHEETS[SHEET_MOB].sprite_sheet = GameMalloc("",sizeof(Texture2D));
  SpriteLoadSubTextures(MOB_SPRITES,SHEET_MOB, NUM_MOB);
  *SHEETS[SHEET_MOB].sprite_sheet = LoadTextureFromImage(mobsImg);


}

sprite_t* InitSpriteByTag(char *name, SheetID s){
  sprite_t* spr = GameCalloc("InitSprite", 1, sizeof(sprite_t));
  sprite_sheet_data_t* data = &SHEETS[s];

  uint32_t hash = hash_str_32(name);
  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->slice->tag != hash)
      continue;

    spr->slice = data->sprites[i]->slice;
    spr->sheet = data->sprite_sheet;

    spr->is_visible = true;
    spr->offset = spr->slice->offset;
    return spr;
  }

  return NULL;

}


anim_t* AnimGet(anim_player_t* a, hash_key_t key){
  return HashGet(&a->map, key);
}

bool AnimPlayNext(anim_player_t* ap, anim_t* a){

  if(a->next < 0)
    return false;

  anim_t* next = AnimGet(ap, a->next);
  if(!next)
    return false;


  ap->cur_seq = next->seq;

  next->state = ANIM_PLAY;

  return true;
}

Vector2 AnimSize(anim_player_t* a){
  if(a->col_data && a->col_data->shape!=0){
    return VEC_NEW(a->col_data->wid, a->col_data->hei);
  }
  return RectSize(a->anims[a->cur_seq].frames[0].bounds);
}

void AnimSetSequence(anim_player_t* ap, anim_t* a){
  if(ap->cur_seq == a->seq){
    ap->next = a->next;
    a->state = ANIM_PLAY;
    return;
  }

  anim_t* cur = &ap->anims[ap->cur_seq];

  a->state = ANIM_PLAY;
  if(cur->cur_index == 0 && cur->elapsed < 1)
    ap->cur_seq = a->seq;
  else{
    ap->next = a->seq;
  }
}

void AnimAddGroup(anim_player_t* a, sprite_data_t* data, int index, int count){
  anim_t* anim = &a->anims[index];

  sprite_slice_t* s = data->slice;
  anim->duration = s->dur;

  if(data->is_root)
    anim->loop = true;

  anim->angle = s->dir;
  sprite_slice_t* as = &anim->frames[anim->num_frames++];
  memcpy(as, s, sizeof(sprite_slice_t));
}

anim_player_t* InitAnimGroup(char* tag, SheetID s){
  anim_player_t* ap = GameCalloc("InitAnimGroup", 1, sizeof(anim_player_t));
  sprite_sheet_data_t* data = &SHEETS[s];
  uint32_t hash = hash_str_32(tag);

  sprite_data_t *found[MAX_SPRITE_FRAMES] = {0};

  uint64_t groups[MAX_ANIM_GROUPS] = {0};
  int num_groups = 0;
  int count = 0;
  int largest = 0;

  int num_roots = 0;
  sprite_slice_t* roots[24] = {0};
  
  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->slice->tag != hash)
      continue;

    if(data->sprites[i]->slice->group == 0)
      continue;

    if(data->sprites[i]->slice->index > largest)
      largest = data->sprites[i]->slice->index;

    if(data->sprites[i]->slice->index == 0)
      groups[num_groups++] = data->sprites[i]->slice->group;

    if(data->sprites[i]->is_root)
      roots[num_roots++] = data->sprites[i]->slice;

    found[count++] = data->sprites[i];
  }

  if(num_groups == 0)
    return NULL;

  HashInit(&ap->map, next_pow2_int(1 + num_groups));

  ap->anims = GameCalloc("InitAnimGroup", num_groups, sizeof(anim_t));
  for (int i = 0; i < num_groups; i++){
    ap->anims[i].group = groups[i];
    ap->anims[i].seq = i;
    ap->anims[i].cap = 1+largest;
    ap->anims[i].frames = GameCalloc("AnimAddGroup", 1+largest, sizeof(sprite_slice_t));

  }
  largest+=1;
  int index = -1;
  for (int i = 0; i < count; i++){ 
    if(found[i]->slice->index == 0)
      index++;

    AnimAddGroup(ap, found[i], index, largest);
  
  }
  ap->num_seq = num_groups;
 
  ap->col_data = &data->sprites[0]->coll; 
  for(int i = 0; i < num_groups; i++){
    uint64_t root = 0;
    for(int j = 0; j < num_roots; j++){
      if(roots[j]->dir == ap->anims[i].angle){
        root = roots[j]->group;
        break;
      }
    }
    ap->anims[i].next = root;
    if(root >= 0)
      ap->anims[i].on_end = AnimPlayNext;
    
    HashPut(&ap->map, ap->anims[i].group, &ap->anims[i]);
  }
  return  ap;

}

sprite_slice_t* AnimGetFrame(anim_player_t* player){
  anim_t* a = &player->anims[player->cur_seq];

  return &a->frames[a->cur_index];
}

void AnimPlay(anim_player_t* player, float spd){
  anim_t* a = &player->anims[player->cur_seq];

  if(a->state != ANIM_PLAY)
    return;

  a->elapsed++;

  if(a->elapsed >= a->duration){
    a->cur_index++;
    a->elapsed = 0;
  }
  else
    return;

  if(a->cur_index >= a->num_frames){
    a->cur_index = 0;

    if(!a->loop)
      a->state = ANIM_DONE;

    if(!a->loop && a->on_end)
      a->on_end(player, a);
  }
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

  Texture sheet = *SHEETS[s->sheet].sprite_sheet;
  Color col = s->color.a > 0? s->color: WHITE; 
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
    DrawSlice(s->slice, pos, s->rot);

}

void DrawSprite(sprite_t* s){
  if(!s->slice)
    return;

}

void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap){
  if(id == SHEET_MOB)
    DO_NOTHING();

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
    str_to_int(parts[2], &spr->dir);
    spr->dur = sprData.dur; 
    spr->center = center;// Vector2Scale(offset,spr->scale);
    spr->offset = VECTOR2_ZERO;//offset;//center;//Vector2Scale(center,spr->scale);
    spr->bounds = bounds;
 
    if(sprData.mirror){
      spr->center.x = spr->bounds.width - spr->center.x;
      spr->bounds.width*=-1;
    }
   
    sprite_data_t* sdat = GameCalloc("Sub", 1, sizeof(sprite_data_t));
    sdat->slice = spr;
    sdat->coll = sprData.collider;
    sdat->is_root = sprData.is_root;

    SHEETS[id].sprites[SHEETS[id].num_sprites++] = sdat;
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
