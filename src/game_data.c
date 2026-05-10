#include "game_systems.h"
#include "component_define.h"
#include "components.h"

int PHYS_SYS;

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
