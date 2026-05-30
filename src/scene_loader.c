#include "scene_loader.h"
#include "scene_data.h"
#include "asset_define.h"
#include "physics_define.h"
#include "component_define.h"
#include "util_parse.h"

bool SceneLoadByIndex(int index, Scene* out){
  if (!SceneLoadHeader(index, out))
    return false;

  const char* jsonPath = SceneGetJsonPath(index);
  if (jsonPath) {
    cJSON* meta = ParseRoot(jsonPath);
    if (meta) {
      // Apply per-level settings (background, music, etc.)
      out->metadata = meta;
    }
  }

  return true;
}

void SceneLoadResources(void){
  for(int i = 0; i < NUM_RES; i++)
    ResourceLoad(RESOURCES[i]);

}

bool SceneInitWeb(void){
  ResourceInit(NUM_RES);

  for(int i = 0; i < NUM_RES; i++)
    RESOURCES[i].dest = &SHEETS[RESOURCES[i].sheet].texture;
}

bool SceneInit(LoadQueue* l){
  ResourceInit(NUM_RES);

  for(int i = 0; i < NUM_RES; i++){
    strcpy(l->jobs[l->count++].path, RESOURCES[i].name);

    l->jobs[i].dest = &SHEETS[RESOURCES[i].sheet].texture;
  }

  return l->count > 0;
}

int SceneResourePaths(void){
  ResourceInit(NUM_RES);

  return NUM_RES;
}

void SceneSetup(world_t* w, Scene* s){
  for(int i = 0; i < s->entity_count; i++){
    entity_instance_t e = s->entities[i];

    Vector2 pos = VEC_NEW(e.x, e.y);
    Entity spawn = PrefabSpawn(w, e.prefab, pos);
  }

  int *index = GameCalloc("SceneSetup", 1, sizeof(int));
  Vector2 size = VEC_NEW(s->cell_width/4, s->cell_height/4);
  for(int i = 0; i < s->tile_count; i++){
    tile_instance_t t = s->tiles[i];
    char pname[MAX_NAME_LEN];
    strcpy(pname, sub_string(t.name, "_", 0));

    Entity tile = PrefabSpawn(w, pname, VEC_UNSET);
    Vector2 pos = VEC_NEW(t.cell_x * s->cell_width, t.cell_y * s->cell_height);

    position_t* p = ComponentGet(w, tile, POS_ID);

    PositionSet(p, pos);

    char istr[4];
    strcpy(istr, sub_string(t.name, "_", 1));
    str_to_int(istr, index);

    tile_define_t* tdef = &s->tile_defs[*index];
    if(!tdef)
      continue;

    name_t* name = ComponentAdd(w, tile, NAME_ID);
    strcpy(name->display, t.name);
    sprite_t* spr = ComponentGet(w, tile, SPR_ID);
    spr->index = tdef->index;
    spr->sheet_id = tdef->sheet;
    spr->layer = tdef->layer;
    for(int j = 0; j < tdef->num_col; j++){
      int y = i_safe_divide(tdef->col_indexes[j], 4);
      int x = tdef->col_indexes[j] % 4;
      Vector2 spos = VEC_NEW(x * s->cell_width/4, y * s->cell_height/4);

      spos = VEC_ADD(spos, pos);
      Entity col_ent = EntityCreate(&w->manager);
      position_t* colp = ComponentAdd(w, col_ent, POS_ID);
      PositionSet(colp, spos);
      rigid_body_t* rb = ComponentAdd(w, col_ent, PHYS_ID);
      rb->bounds.shape = SHAPE_REC;
      rb->is_static = true;
      RigidBodySetBounds(rb, size);
      RigidBodySetPos(rb, spos);
      RigidBodySetOffset(rb, VEC_SCALE(size,2));

      rb->on_coll = PHYS_EVENT_BLOCK;
    }

  }
  GameFree("SceneSetup", index);
}
