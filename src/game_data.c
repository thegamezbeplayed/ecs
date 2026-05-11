#include "game_systems.h"
#include "component_define.h"
#include "components.h"
<<<<<<< HEAD

const component_define_t CORE_COMPONENTS[NUM_COMP_CORE] = {
  {"Position",  sizeof(position_t)},
  {"RigidBody", sizeof(rigid_body_t)},
  {"Animation", sizeof(anim_comp_t)},
  {"Sprite",    sizeof(sprite_t)},
  {"Input",     sizeof(input_t)},
  {"Camera",    sizeof(cam_comp_t)},
  {"Track",     sizeof(track_comp_t)},
  {"Type",      sizeof(EntityType)},
  {"Stat",      sizeof(stat_t)},
  {"Force",     sizeof(force_t)},
  {"Name",      0}, //TODO
  {"State",     sizeof(state_comp_t)},
  {"Follow",    sizeof(follow_comp_t)},
  {"Level",     sizeof(level_t)},
  {"Expiry",    sizeof(lifetime_t)}
};
=======
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

const component_func_t COMPFUNC_LOOKUP[NUM_COMP_CORE] = {
  {"Sprite",      SpriteInit},
  {"Anim",        AnimInit},
  {"RigidBody",   RigidBodyInit},
  {"Force",       ForceInit},
  {"Camera",      CameraInit},
  {"Position",    PositionInit},
  {"Coordinate",  CoordInit}
};

<<<<<<< HEAD
component_entry_t* GetGameComponentDefine(game_t* g, const char* name){
  for(int i = 0; i < g->num_comps; i++){
    component_entry_t* entry = &g->comp_defs[i];
    if(strcmp(name, entry->name) == 0)
      return entry;
  }

  return NULL;
}

void GameInitPrefabs(world_t* w, game_t* g){
  PrefabRegistryInit(w);
  
  for (int i = 0; i < g->num_prefabs; i++){
    prefab_entity_t def = g->prefabs[i];
    Entity prefab = PrefabCreate(w, def.name);

    for(int j = 0; j < def.num_comp; j++){
      comp_id_t cid = ComponentGetID(def.components[j]);

      if(cid == INVALID_COMPONENT)
        continue;

      ComponentInitFn fn = ComponentFuncLookup(def.components[j]);
     
      if(!fn)
       continue;

      component_entry_t *data = GetGameComponentDefine(g, def.name);
      if(data)
        fn(ComponentAdd(w, prefab, cid), data);
    }
  }

}

void GameSpawn(world_t* w, game_t* g){
  for(int i = 0; i < w->prefabs.count; i++){
    prefab_t* prefab = &w->prefabs.prefabs[i];
    if(prefab)
      PrefabSpawn(w, prefab->name, VEC_UNSET); 
  }
}
=======
void RegisterSystemData(world_t* w){
  SystemCB ltick[UPDATE_DONE] = {0};
  SystemCB lset[GAME_DONE] = {0};

  lset[GAME_LOADING] = LevelLoad;
  lset[GAME_READY] = LevelReady;

  ltick[UPDATE_DRAW] = LevelRender;
  system_t* lvlsys = SystemRegister(w, ltick, lset, NULL);
  SystemRequire(lvlsys, LVL_ID);

  SystemCB rntick[UPDATE_DONE] = {0};
  rntick[UPDATE_DRAW_BEGIN] = RenderBegin;
  rntick[UPDATE_DRAW_END] = RenderEnd;

  SystemCB rnset[GAME_DONE] = {0};
  rnset[GAME_READY] = RenderLoad;

  system_t* rnsys = SystemRegister(w, rntick, rnset, NULL);

  SystemRequire(rnsys, CAM_ID);

  SystemCB cftick[UPDATE_DONE] = {0};
  cftick[UPDATE_POST] = CameraSystem;

  SystemCB cfset[GAME_DONE] = {0};
  cfset[GAME_LOADING] = CameraLoad;
  cfset[GAME_READY] = CameraReady;

  system_t* cfsys = SystemRegister(w, cftick, cfset, NULL);
  SystemRequire(cfsys, CAM_ID);
  SystemRequire(cfsys, TRACK_ID);

  SystemCB sptick[UPDATE_DONE] = {0};
  sptick[UPDATE_DRAW] = SpriteRender;

  SystemCB spset[GAME_DONE] = {0};

  system_t* spsys = SystemRegister(w, sptick, spset, NULL);

  SystemRequire(spsys, SPR_ID);
  SystemRequire(spsys, POS_ID);

  SystemCB frtick[UPDATE_DONE] = {0};
  frtick[UPDATE_FIXED] = ForceSystem;
  frtick[UPDATE_FINAL] = ForceCleanup;

  SystemCB frset[GAME_DONE] = {0};
  frset[GAME_READY] = ForceLoad;

  system_t* frsys = SystemRegister(w, frtick, frset, NULL);

  SystemRequire(frsys, FORCE_ID);

  SystemCB lftick[UPDATE_DONE] = {0};
  lftick[UPDATE_FINAL] = ExpirationSystem;

  SystemCB lfset[GAME_DONE] = {0};

  system_t* lfsys = SystemRegister(w, lftick, lfset, NULL);

  SystemRequire(lfsys, EXPIR_ID);


  /*
  SystemCB cmbtick[UPDATE_DONE] = {0};
  cmbtick[UPDATE_FIXED] = CombatSystem;

  SystemCB cmbset[GAME_DONE] = {0};
  cmbset[GAME_READY] = CombatLoad;

  system_t* cmbsys = SystemRegister(w, cmbtick, cmbset);

  SystemRequire(cmbsys, );
*/
}

const component_define_t CORE_COMPONENTS[NUM_COMP_CORE] = {
  {"Position",  sizeof(position_t)},
  {"Physics",   sizeof(rigid_body_t)},
  {"Animation", sizeof(anim_comp_t)},
  {"Sprite",    sizeof(sprite_t)},
  {"Input",     sizeof(input_t)},
  {"Camera",    sizeof(cam_comp_t)},
  {"Track",     sizeof(track_comp_t)},
  {"Type",      sizeof(EntityType)},
  {"Stat",      sizeof(stat_t)},
  {"Force",     sizeof(force_t)},
  {"Name",      0}, //TODO
  {"State",     sizeof(state_comp_t)},
  {"Follow",    sizeof(follow_comp_t)},
  {"Level",     sizeof(level_t)},
  {"Expiry",    sizeof(lifetime_t)}
};
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
