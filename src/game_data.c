#include "game_systems.h"
#include "scene_data.h"

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

int PHYS_SYS;

void RegisterComponentData(world_t* w) {
  ANIM_ID = REGISTER_COMPONENT(w, anim_comp_t);
  ComponentMap("Animation", &ANIM_ID, AnimationImport);
  
  NAME_ID = REGISTER_COMPONENT(w, name_comp_t);
  ComponentMap("Name", &NAME_ID, NULL);
  POS_ID = REGISTER_COMPONENT(w, pos_comp_t);
  ComponentMap("Position", &POS_ID, PositionImport);

  INPUT_ID = REGISTER_COMPONENT(w, input_comp_t);
  ComponentMap("Input", &INPUT_ID, InputImport);
  PHYS_ID = REGISTER_COMPONENT(w, phys_comp_t);//DuplicateRigidBody);
  ComponentMap("Physics", &PHYS_ID, PhysicsImport);
  LVL_ID = REGISTER_COMPONENT(w, lvl_comp_t);
  ComponentMap("Level", &LVL_ID, LevelImport);
  CAM_ID = REGISTER_COMPONENT(w, cam_comp_t);
  ComponentMap("Camera", &CAM_ID, CameraImport);
  TRACK_ID = REGISTER_COMPONENT(w, track_comp_t);
  ComponentMap("Track", &TRACK_ID, TrackingImport);
  SPR_ID = REGISTER_COMPONENT(w, spr_comp_t);
  ComponentMap("Sprite", &SPR_ID, SpriteImport);
  TYPE_ID = REGISTER_COMPONENT(w, type_comp_t);
  ComponentMap("Type", &TYPE_ID, TypeImport);

  FOLLOW_ID = REGISTER_COMPONENT(w, follow_comp_t);
  ComponentMap("Follow", &FOLLOW_ID, NULL);

  AI_ID = REGISTER_COMPONENT(w, ai_comp_t);

  STATE_ID = REGISTER_COMPONENT(w, state_comp_t);
  
  STAT_ID = REGISTER_COMPONENT(w, stat_comp_t);
  ComponentMap("Stat", &STAT_ID, StatImport);
}

void RegisterSystemData(world_t* w){
  SystemCB atick[UPDATE_DONE] = {0};
  atick[UPDATE_FIXED] = AnimSystem;
  atick[UPDATE_DRAW] = AnimRender;

  SystemCB aset[GAME_DONE] = {0};

  aset[GAME_READY] = AnimLoad;

  system_t* asys = SystemRegister(w, atick, aset);

  SystemRequire(asys, ANIM_ID);
  SystemRequire(asys, POS_ID);

  SystemCB intick[UPDATE_DONE] = {0};
  intick[UPDATE_FRAME] = InputSystem;

  SystemCB inset[GAME_DONE] = {0};
  inset[GAME_READY] = InputLoad;
  system_t* insys = SystemRegister(w, intick, inset);

  SystemRequire(insys, INPUT_ID);
  SystemRequire(insys, POS_ID);

  SystemCB potick[UPDATE_DONE] = {0};

  SystemCB poset[GAME_DONE] = {0};
  poset[GAME_READY] = PositionLoad;
  system_t* posys = SystemRegister(w, potick, poset);
  SystemRequire(posys, POS_ID);

  SystemCB phtick[UPDATE_DONE] = {0};
  phtick[UPDATE_PRE] = PhysicsCollision;
  phtick[UPDATE_POST] = PhysicsSystem;

  SystemCB phset[GAME_DONE] = {0};
  phset[GAME_READY] = PhysicsLoad;
  system_t* phsys = SystemRegister(w, phtick, phset);

  PHYS_SYS = phsys->index;
  SystemRequire(phsys, PHYS_ID);
  SystemRequire(phsys, POS_ID);

  SystemCB ltick[UPDATE_DONE] = {0};
  SystemCB lset[GAME_DONE] = {0};

  lset[GAME_LOADING] = LevelLoad;
  lset[GAME_READY] = LevelReady;

  ltick[UPDATE_DRAW] = LevelRender;
  system_t* lvlsys = SystemRegister(w, ltick, lset);
  SystemRequire(lvlsys, LVL_ID);

  SystemCB rntick[UPDATE_DONE] = {0};
  rntick[UPDATE_DRAW_BEGIN] = RenderBegin;
  rntick[UPDATE_DRAW_END] = RenderEnd;

  SystemCB rnset[GAME_DONE] = {0};
  rnset[GAME_READY] = RenderLoad;

  system_t* rnsys = SystemRegister(w, rntick, rnset);

  SystemRequire(rnsys, CAM_ID);

  SystemCB cftick[UPDATE_DONE] = {0};
  cftick[UPDATE_POST] = CameraSystem;

  SystemCB cfset[GAME_DONE] = {0};
  cfset[GAME_LOADING] = CameraLoad;
  cfset[GAME_READY] = CameraReady;

  system_t* cfsys = SystemRegister(w, cftick, cfset);
  SystemRequire(cfsys, CAM_ID);
  SystemRequire(cfsys, TRACK_ID);

  SystemCB sptick[UPDATE_DONE] = {0};
  sptick[UPDATE_DRAW] = SpriteRender;

  SystemCB spset[GAME_DONE] = {0};

  system_t* spsys = SystemRegister(w, sptick, spset);

  SystemRequire(spsys, SPR_ID);
  SystemRequire(spsys, POS_ID);

  SystemCB cbtick[UPDATE_DONE] = {0};
  cbtick[UPDATE_FIXED] = CombatSystem;

  SystemCB cbset[GAME_DONE] = {0};

  system_t* cbsys = SystemRegister(w, cbtick, cbset);

  SystemRequire(cbsys, STAT_ID);
  SystemRequire(cbsys, PHYS_ID);
  SystemRequire(cbsys, ANIM_ID);

}
