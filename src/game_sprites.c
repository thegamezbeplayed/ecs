#include "game_assets.h"
#include "game_strings.h"
//#include "game_math.h"
#include "game_tools.h"
#include "game_define.h"
#include "game_process.h"
#include "game_helpers.h"
#include "component_define.h"

//#include "screens.h"

sprite_sheet_d SHEETS[SHEET_ALL];
static splash_tex_t splash_tex;

void SpriteLoadSplash(const char* path, Vector2 screen){
  splash_tex.tex = GameMalloc("SpriteLoadSplash", sizeof(Texture2D));
  *splash_tex.tex = LoadTexture(path);

  splash_tex.offset.x = (screen.x - splash_tex.tex->width)/2;
  splash_tex.offset.y = (screen.y - splash_tex.tex->height)/2;

  splash_tex.bg = (Color){9,13,24,255};
}

void SpriteRenderSplash(void){
  ClearBackground(splash_tex.bg);

  DrawTextureV(*splash_tex.tex, splash_tex.offset, WHITE);
}

void InitResources(){
  sprite_sheet_d *s = &SHEETS[SHEET_UI];
  s->texture = LoadTexture("resources/textures_npatch.png");
}

sprite_t* InitSprite(SheetID s, int index){
  sprite_t* spr = GameCalloc("InitSprite", 1, sizeof(sprite_t));
  sprite_sheet_d* data = &SHEETS[s];

  spr->sheet_id = s;
  spr->index = index;

  return spr;
}

collision_d* InitSpriteCollision(anim_frame_t* f, CollType coll, ShapeType s, Rectangle r){

  collision_d* c = GameCalloc("InitSpriteCollision", 1, sizeof(collision_d));
  c->frame = f->index;
  c->type = coll;
  c->shape = s;
  c->posx = r.x - f->source_rect.x;
  c->posy = r.y - f->source_rect.y;
  c->wid = r.width;
  c->hei = r.height;

  return c;
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

void DrawSprite(sprite_t* spr, Vector2 pos){
  sprite_slice_t* s  = &SHEETS[spr->sheet_id].sprites[spr->index].slice;
  Rectangle src = s->bounds;

  Vector2 origin = (Vector2){
    s->center.x * spr->scale,//offset.x,
      s->center.y * spr->scale//offset.y
  };

  Vector2 position = Vector2Add(pos,s->center);
  Rectangle dst = {
    position.x,
    position.y,
    abs(s->bounds.width * spr->scale),
    s->bounds.height * spr->scale
  };

  Texture sheet = SHEETS[s->sheet].texture;
  Color col = s->color.a > 0? s->color: WHITE;
  DrawTexturePro(sheet,src,dst, origin, spr->rot, col);

  return;



}

void SpriteLoadSubTextures(sub_texture_t* data, SheetID id, int sheet_cap){
  /*
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
  */
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

AnimEventID AnimPlay(anim_t* a){

  AnimEventID ev = 0;
  if(a->elapsed >= a->duration){
    ev = ANIM_EVENT_FRAME_START;
    a->cur_index++;
    a->elapsed = 0;
  }
  else{
    a->elapsed++;
    return ev;
  }

  if(a->cur_index >= a->count){
    a->cur_index = 0;
    if(!a->loop)
      ev = ANIM_EVENT_SEQ_END;
  }

  return ev;
}

bool AnimSetState(anim_t* a, AnimState s){
  if(a->state != s)
    a->state = s;

  return a->state == s;
}

bool AnimPlayerState(anim_player_t* player, anim_t* cur, AnimState s){
  if(s == ANIM_NONE || player->state == s)
    return false;

  if(!cur->interupt)
    return false;

  player->state = s;

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

    if(SHEETS[id].sprites[i].group != ghash)
      continue;

    sprite_slice_t* spr = &SHEETS[id].sprites[i].slice;
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

Rectangle GetSlice(const ase_sprite_sheet_d* sheet, const char* slice_name, int frame)
{
    if (!sheet || !slice_name) return (Rectangle){0};

    for (int i = 0; i < sheet->num_slices; i++) {
        const slice_d* s = &sheet->slices[i];
        if (strcmp(s->name, slice_name) != 0)
            continue;

        for (int k = 0; k < s->num_keys; k++) {
            if (s->keys[k].frame == frame) {
                return s->keys[k].bounds;
            }
        }
    }
    return (Rectangle){0, 0, 0, 0};
}

// Convenience wrappers
bool IsHitboxActive(const ase_sprite_sheet_d* sheet, int frame)
{
    Rectangle r = GetSlice(sheet, "hitbox", frame);
    return r.width > 0 && r.height > 0;
}

Rectangle GetHurtbox(const ase_sprite_sheet_d* sheet, int frame)
{
    Rectangle r = GetSlice(sheet, "hurtbox", frame);
    return r;
}

bool SpriteInit(void* comp, component_entry_t* j){
  sprite_t* spr = comp;

  return ParseSpriteComponent(j->data, spr);
}

bool AnimInit(void* comp, component_entry_t* j){
  anim_comp_t* ac = comp;

  return ParseAnimComponent(j->data, ac);
}
