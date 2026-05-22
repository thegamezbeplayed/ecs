#include "game_define.h"
#include "asset_resources.h"
#include "gbm_paths.h"
#include "asset_define.h"
#include "system_define.h"
#include "util_parse.h"
#include "tool_lookup.h"

static ResourcePool RES_POOL;
static cJSON* comp;
static cJSON* sys;
static cJSON* pref;
static bool s_load, c_load, p_load, r_load;

void AsepriteToAnim(SheetID id, const ase_sprite_sheet_d* ase, anim_tag_t tag, int index, sprite_d* out){
  if (!ase || !out || index < 0 || index >= ase->num_frames) {
    memset(out, 0, sizeof(sprite_d));
    return;
  }
    const anim_frame_t* f = &ase->frames[index];

    // Basic info 
    out->tag   = tag.hash;
    out->sheet_index = index;
    out->sheet_id = id;
    out->duration = f->duration;
    out->slice.scale = 1.0f;
    out->group = tag.group;
  
    strcpy(out->name, tag.name);
    out->repeat = tag.repeat;
    
    if(strcmp(tag.direction, "reverse") == 0)
      out->mirror = true;
    out->slice.bounds = f->frame_rect;

    out->slice.sheet = id;
    out->slice.center = (Vector2){
        f->source_rect.width  * 0.5f,
        f->source_rect.height * 0.5f
    };

    if(out->mirror){
      out->slice.center.x = out->slice.bounds.width - out->slice.center.x;      out->slice.bounds.width*=-1;
    }
}

void AsepriteToSprite(SheetID id, const ase_sprite_sheet_d* ase, int index, sprite_d* out){
  if (!ase || !out || index < 0 || index >= ase->num_frames) {
    memset(out, 0, sizeof(sprite_d));
    return;
  }

  const anim_frame_t* f = &ase->frames[index];

  out->sheet_id     = id;
  out->sheet_index  = index;
  out->slice.scale  = 1.0f;
  out->slice.sheet  = id;
  out->slice.bounds = f->frame_rect;
  out->slice.center = VEC_NEW(f->source_rect.width  * 0.5f, f->source_rect.height * 0.5f);

}

void ParseHitboxFromAseprite(cJSON* s, cJSON* k, collision_d* out, cJSON* f)
{
    cJSON* bounds = cJSON_GetObjectItem(k, "bounds");
    if (!bounds) return;

    char sname[MAX_NAME_LEN];

    Json_GetString(s, "name", sname);    
    out->type = StringToCollType(sname);

    out->frame = Json_GetInt(k, "frame", -1);
    out->shape = SHAPE_REC;
    // Raw slice position (relative to trimmed frame)
    float slice_x = cJSON_GetObjectItem(bounds, "x")->valuedouble;
    float slice_y = cJSON_GetObjectItem(bounds, "y")->valuedouble;
    float slice_w = cJSON_GetObjectItem(bounds, "w")->valuedouble;
    float slice_h = cJSON_GetObjectItem(bounds, "h")->valuedouble;

    // Get sprite trim offset
    cJSON* srcSize = cJSON_GetObjectItem(f, "spriteSourceSize");
    float trim_x = cJSON_GetObjectItem(srcSize, "x")->valuedouble;
    float trim_y = cJSON_GetObjectItem(srcSize, "y")->valuedouble;

    // Convert to sprite-local coordinates
    out->x     = slice_x - trim_x;
    out->y     = slice_y - trim_y;
    out->wid   = slice_w;
    out->hei   = slice_h;
}

void ResourceMapAsepriteAnims(SheetID id, sprite_sheet_d* s, ase_sprite_sheet_d* ase){
  for(int i = 0; i < ase->num_tags ; i++){
    anim_tag_t tag = ase->tags[i];
    for(int j = tag.index_start; j <= tag.index_end; j++){
      AsepriteToAnim(id, ase, tag, j, &s->sprites[j]);
      s->num_sprites++;
    }
  }

  if(!ase->frame_meta || !ase->slice_meta)
    return;

  cJSON* slice;
  cJSON_ArrayForEach(slice, ase->slice_meta){
    cJSON* keys = cJSON_GetObjectItem(slice, "keys");
    cJSON* key;
    cJSON_ArrayForEach(key, keys){
      int fidx = Json_GetInt(key, "frame", -1);
      if( fidx == -1)
        continue;

      cJSON* frame = cJSON_GetArrayItem(ase->frame_meta, fidx);
      if(!frame)
        continue;

      ParseHitboxFromAseprite(slice, key, &s->coll[s->num_coll++], frame);
    }
  }
}

void ResourceMapAseprites(SheetID id, sprite_sheet_d* s, ase_sprite_sheet_d* ase){

  int i = 0;
  for(i; i < ase->num_frames; i){
    AsepriteToSprite(id, ase, i++, &s->sprites[s->num_sprites++]);
  }

  if(i == 0)
    TraceLog(LOG_WARNING, "=== RES: MAP ASEPRITE ===\n No Aseprite assets loaded!"); 
}

void ResourceLoadAseprite(cJSON* root, ResourceRef* ref){
  ase_sprite_sheet_d* parse = GameCalloc("InitResources", 1,
      sizeof(ase_sprite_sheet_d));

  if(!LoadAsepriteSheet(root, parse))
    return;

  sprite_sheet_d *s = &SHEETS[ref->sheet];

  switch(ref->subtype){
    case RES_ANIM:
      ResourceMapAsepriteAnims(ref->sheet, s, parse);
      break;
    case RES_SPRITE:
      ResourceMapAseprites(ref->sheet, s, parse);
      break;
  }

}

void ResourceLoadJSON(ResourceRef* ref){
  cJSON* root = ParseRoot(ref->path);

  if(!root)
    return;

  switch(ref->type){
    case RES_ASEPRITE:
      ResourceLoadAseprite(root, ref);
      break;

  }
}

void ResourceInit(int count){
  RES_POOL.cap = count;
  RES_POOL.refs = GameCalloc("ResourceInit", count, sizeof(ResourceRef));
}

bool LoadGameDefine(game_t* g){
  comp = ParseRoot(PATH_DEF_COMP);
  if(comp)
    c_load = ParseComponents(comp, g);
  
  if(!c_load){
    TraceLog(LOG_WARNING, "=== GAME COMPONENTS NOT LOADED===");
    return false;
  }

  sys = ParseRoot(PATH_DEF_SYST);
  if(sys)
    s_load = ParseSystems(sys, g);

  if(!s_load){
    TraceLog(LOG_WARNING, "=== GAME SYSTEMS NOT LOADED===");
    return false;
  }

  pref = ParseRoot(PATH_DEF_PREF);
  if(pref){
    p_load = ParsePrefabs(pref, g);
    r_load = ParseRelations(pref, g);
  }

  if(!p_load){
    TraceLog(LOG_WARNING, "=== GAME PREFABS NOT LOADED===");
    return false;
  }
  
  if(!r_load){
    TraceLog(LOG_WARNING, "=== GAME RELATIONS NOT LOADED===");
    return false;
  }

  return c_load && s_load && p_load && r_load;
}

void ResourceLoad(ResourceRef ref){
  ResourceRef *entry = &RES_POOL.refs[RES_POOL.count++];

  *entry = ref;

  switch(entry->cat){
    case RES_JSON:
      ResourceLoadJSON(entry);
  }
}

void UnloadGameDefine(game_t* g){
  if(!g)
    return;

  for (int i = 0; i < g->num_prefabs; i++) {
    prefab_entity_t* p = &g->prefabs[i];
    if(p->name)
      GameFree("UnloadGameDefine", (void*)p->name);
    for (int j = 0; j < g->prefabs[i].num_comp; j++) {
      GameFree("UnloadGameDefine", (void*)p->components[j]);
    }
  }

  // Relations
  for (int i = 0; i < g->relation_count; i++) {
    entity_relation_t* er = &g->relations[i];
    GameFree("UnloadGameDefine", (void*)er->name);

    for (int j = 0; j < MAX_RELATIONS; j++) {
      component_relation_t* cr = &er->comps[j];
      GameFree("UnloadGameDefine", (void*)cr->comp);

      for (int k = 0; k < MAX_RELATIONS_PER_ENTITY; k++) {
        GameFree("UnloadGameDefine", (void*)cr->pairs[k].name);
      }
    }
  }
  GameFree("UnloadGameDefine", g->relations);
  memset(g, 0, sizeof(*g));

  if (comp) cJSON_Delete(comp);
  if (sys) cJSON_Delete(sys);
}
