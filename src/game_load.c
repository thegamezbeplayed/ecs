#include "game_assets.h"
#include "game_systems.h"

static ResourcePool RES_POOL;
static game_t       GAME_DEF;
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

void ResourceMapAsepriteAnims(sprite_sheet_d* s, ase_sprite_sheet_d* ase){
  for(int i = 0; i < ase->num_tags ; i++){
    anim_tag_t tag = ase->tags[i];
    for(int j = tag.index_start; j <= tag.index_end; j++){
      AsepriteToAnim(SHEET_CHAR, ase, tag, j, &s->sprites[j]);
      s->num_sprites++;
    }
  }

  for (int i = 0; i < ase->num_slices; i++){
    slice_d sdat = ase->slices[i];
    for (int j = 0; j < sdat.num_keys; j++){
      slice_key_t skdat = sdat.keys[j];

      anim_frame_t* f = &ase->frames[skdat.frame];
      s->coll[s->num_coll++] = *InitSpriteCollision(f, skdat.type,
          SHAPE_REC, skdat.bounds);
    }
  }
}

void ResourceMapAseprites(SheetID id, sprite_sheet_d* s, ase_sprite_sheet_d* ase){

  for(int i = 0; i < ase->num_frames; i++){
    AsepriteToSprite(id, ase, i, &s->sprites[s->num_sprites++]);
  }
}

void ResourceLoadAseprite(cJSON* root, ResourceRef* ref){
  ase_sprite_sheet_d* parse = GameCalloc("InitResources", 1,
      sizeof(ase_sprite_sheet_d));

  if(!LoadAsepriteSheet(root, parse))
    return;

  sprite_sheet_d *s = &SHEETS[ref->sheet];

  switch(ref->subtype){
    case RES_ANIM:
      ResourceMapAsepriteAnims(s, parse);
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

game_t* LoadGameDefine(const char* path){
  cJSON* root = ParseRoot(path);

  bool load = ParseGameDefinition(root, &GAME_DEF);

  if(!load){
    TraceLog(LOG_WARNING, "=== GAME DEF NOT LOADED===");
    return NULL;
  }

  ComponentInit(GAME_DEF.num_comps);
  for(int i = 0; i < GAME_DEF.num_comps; i++)
    ComponentRegisterCore(GAME_DEF.comps[i]);

  for(int i = 0; i < GAME_DEF.num_sys; i++)
    SystemCreate(&world, &GAME_DEF.systems[i]);

  return &GAME_DEF;
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
  GameFree("UnloadGameDefine", g->prefabs);

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


}
