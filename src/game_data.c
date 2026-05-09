#include "game_systems.h"

uint64_t AI_ID;
uint64_t ANIM_ID;
uint64_t NAME_ID;
uint64_t POS_ID;
uint64_t INPUT_ID;
uint64_t PHYS_ID;
uint64_t LVL_ID;
uint64_t CAM_ID;
uint64_t TRACK_ID;
uint64_t SPR_ID;
uint64_t TYPE_ID;
uint64_t FOLLOW_ID;
uint64_t STATE_ID;
uint64_t STAT_ID;
uint64_t FORCE_ID;
uint64_t EXPIR_ID;

int PHYS_SYS;

void RegisterComponentData(world_t* w) {
  ANIM_ID = REGISTER_COMPONENT(w, anim_comp_t);
  
  NAME_ID = REGISTER_COMPONENT(w, name_comp_t);
  POS_ID = REGISTER_COMPONENT(w, pos_comp_t);

  INPUT_ID = REGISTER_COMPONENT(w, input_comp_t);
  PHYS_ID = REGISTER_COMPONENT(w, phys_comp_t);//DuplicateRigidBody);
  LVL_ID = REGISTER_COMPONENT(w, lvl_comp_t);
  CAM_ID = REGISTER_COMPONENT(w, cam_comp_t);
  TRACK_ID = REGISTER_COMPONENT(w, track_comp_t);
  SPR_ID = REGISTER_COMPONENT(w, spr_comp_t);
  TYPE_ID = REGISTER_COMPONENT(w, type_comp_t);

  FOLLOW_ID = REGISTER_COMPONENT(w, follow_comp_t);

  AI_ID = REGISTER_COMPONENT(w, ai_comp_t);

  STATE_ID = REGISTER_COMPONENT(w, state_comp_t);
  
  STAT_ID = REGISTER_COMPONENT(w, stat_comp_t);

  FORCE_ID = REGISTER_COMPONENT(w, force_comp_t);

  EXPIR_ID = REGISTER_COMPONENT(w, lifetime_t);
   
}

void RegisterSystemData(world_t* w){
  SystemCB atick[UPDATE_DONE] = {0};
  //atick[UPDATE_PRE] = AnimBehavior;
  atick[UPDATE_FIXED] = AnimSystem;
  atick[UPDATE_DRAW] = AnimRender;

  SystemCB aset[GAME_DONE] = {0};

  aset[GAME_READY] = AnimLoad;

  system_t* asys = SystemRegister(w, atick, aset, NULL);

  SystemRequire(asys, ANIM_ID);
  SystemRequire(asys, POS_ID);

  SystemCB intick[UPDATE_DONE] = {0};
  intick[UPDATE_FRAME] = InputSystem;

  SystemCB inset[GAME_DONE] = {0};
  inset[GAME_READY] = InputLoad;
  system_t* insys = SystemRegister(w, intick, inset, NULL);

  SystemRequire(insys, INPUT_ID);
  SystemRequire(insys, POS_ID);

  SystemCB potick[UPDATE_DONE] = {0};

  SystemCB poset[GAME_DONE] = {0};
  poset[GAME_READY] = PositionLoad;
  system_t* posys = SystemRegister(w, potick, poset, NULL);
  SystemRequire(posys, POS_ID);

  SystemCB phtick[UPDATE_DONE] = {0};
  phtick[UPDATE_PRE] = PhysicsCollision;
  phtick[UPDATE_POST] = PhysicsSystem;
  phtick[UPDATE_DRAW] = PhysicsDebug;

  SystemCB phset[GAME_DONE] = {0};
  phset[GAME_READY] = PhysicsLoad;
  system_t* phsys = SystemRegister(w, phtick, phset, PhysicsInit);
  phsys->needs_iter = true;
  PHYS_SYS = phsys->index;
  SystemRequire(phsys, PHYS_ID);
  SystemRequire(phsys, POS_ID);

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
