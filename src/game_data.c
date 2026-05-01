#include "game_define.h"
#include "game_systems.h"
#include "game_register.h"

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
  //cfset[GAME_READY] = RenderLoad;

  system_t* cfsys = SystemRegister(w, cftick, cfset);
  SystemRequire(cfsys, CAM_ID);
  SystemRequire(cfsys, TRACK_ID);

  SystemCB sptick[UPDATE_DONE] = {0};
  sptick[UPDATE_DRAW] = SpriteRender;

  SystemCB spset[GAME_DONE] = {0};

  system_t* spsys = SystemRegister(w, sptick, spset);

  SystemRequire(spsys, SPR_ID);
  SystemRequire(spsys, POS_ID);

}
void RegisterPlayerData(world_t* w){
  Entity p = EntityCreate(&w->manager);
  PLAYER = p.id;

  anim_comp_t* ac = ComponentAdd(w, p, ANIM_ID);

  ac->player.sheet_id = SHEET_CHAR;
  for(int i = 0; i < MAX_DIRECTIONS; i++){

    char gname[MAX_NAME_LEN] = {0};

    int slen = sprintf(gname,"idle%i",i*90);
    ac->sequences[ANIM_IDLE][i] = *AnimRegisterState(SHEET_CHAR, "player", gname);
    ac->sequences[ANIM_IDLE][i].loop = true;
 

    slen = sprintf(gname,"walk%i", i*90);
    ac->sequences[ANIM_WALK][i] = *AnimRegisterState(SHEET_CHAR, "player", gname);

    ac->sequences[ANIM_WALK][i].on_end = AnimIdle;
  }

  pos_comp_t* pc = ComponentAdd(w, p, POS_ID);
  pc->pos = *InitPosition(CELL_NEW(15,10));

  input_comp_t* ic = ComponentAdd(w, p, INPUT_ID);
  ic->input = *InitInput();

  phys_comp_t* phc = ComponentAdd(w, p, PHYS_ID);

  Vector2 size = VEC_BOTH(24);

  phc->rb = *InitRigidBody(pc->pos.vpos, SHAPE_CIRCLE, size.x, size.y);
  
  force_t* bump = ForceBump(VEC_BOTH(0.25f));

  RigidBodyGiveForce(&phc->rb, bump);

  force_t* steer = ForceFromVec2(FORCE_STEERING, VEC_BOTH(0.175f));

  steer->max_velocity = 3.4f;
  steer->friction = VEC_BOTH(.625f);
  steer->threshold = 0.0337f;
  RigidBodyGiveForce(&phc->rb, steer);

  name_comp_t* nc = ComponentAdd(w, p, NAME_ID);

  strncpy(nc->name, "Guy", MAX_NAME_LEN);
  nc->name[MAX_NAME_LEN - 1] = '\0';

}

void RegisterPrefabData(world_t* w){
  PrefabRegistryInit(w);

  Entity slime = PrefabCreate(w, "Slime");

  anim_comp_t* ac = ComponentAdd(w, slime, ANIM_ID);
  ac->player.sheet_id = SHEET_MOB;
 
  type_comp_t *stc = ComponentAdd(w, slime, TYPE_ID);
  stc->type = ENT_MOB; 
  for(int i = 0; i < MAX_DIRECTIONS; i++){

    char gname[MAX_NAME_LEN] = {0};

    int slen = sprintf(gname,"idle%i",i*90);
    ac->sequences[ANIM_IDLE][i] = *AnimRegisterState(SHEET_MOB, "slime", gname);
    ac->sequences[ANIM_IDLE][i].loop = true;


    slen = sprintf(gname,"walk%i", i*90);
    ac->sequences[ANIM_WALK][i] = *AnimRegisterState(SHEET_MOB, "slime", gname);

    ac->sequences[ANIM_WALK][i].on_end = AnimIdle;
  }

  pos_comp_t* pc = ComponentAdd(w, slime, POS_ID);
  pc->pos = *InitPosition(CELL_NEW(0, 0));


  phys_comp_t* phc = ComponentAdd(w, slime, PHYS_ID);
  Vector2 size = VEC_BOTH(16);

  phc->rb = *InitRigidBody(pc->pos.vpos, SHAPE_CIRCLE, size.x, size.y);

  force_t* bump = ForceBump(VEC_BOTH(0.175f));

  force_t* steer = ForceFromVec2(FORCE_STEERING, VEC_BOTH(0.175f));

  steer->max_velocity = .975f;
  steer->friction = VEC_BOTH(.47f);
  steer->threshold = 0.067f;
  RigidBodyGiveForce(&phc->rb, steer);
  RigidBodyGiveForce(&phc->rb, bump);

  Entity fence = PrefabCreate(w, "Fence");

  spr_comp_t *sc = ComponentAdd(w, fence, SPR_ID);

  sc->sprite = *InitSprite(SHEET_OBJ, 0);

  pos_comp_t *fpos = ComponentAdd(w, fence, POS_ID);

  type_comp_t *ftc = ComponentAdd(w, fence, TYPE_ID);
  ftc->type = ENT_OBJ;

  phys_comp_t* frb  = ComponentAdd(w, fence, PHYS_ID);

  Vector2 border = ROOM_SIZE;
  frb->rb = *InitRigidBody(pc->pos.vpos, SHAPE_REC, border.x, border.y); 

  frb->rb.is_static = true;
  frb->rb.col_rate  = 1;
  frb->rb.restitution  = 0.25f;

  force_t* block = ForceFromVec2(FORCE_AVOID, VEC_BOTH(0.175f));
  block->on_react = ForceReactBlock;
  RigidBodyGiveForce(&frb->rb, block);
}

void RegisterWorldData(world_t* w){
  Entity l = EntityCreate(&w->manager);
  lvl_comp_t* lc = ComponentAdd(w, l, LVL_ID);

  lc->wid = 800;
  lc->hei = 600;
  Entity c = EntityCreate(&w->manager);
  cam_comp_t* cc = ComponentAdd(w, c, CAM_ID);

  cc->camera = *InitCamera(3.f, 0, VEC_SCALE(ROOM_SIZE, 0.5f));
  Rectangle border = Rect(264, 200, 272, 254);
  float dist = 0;
  cc->view   = *InitView(ROOM_SIZE, border, dist);

  track_comp_t* tc = ComponentAdd(w, c, TRACK_ID);
  tc->ctx    = *InitCameraContext(CAM_FOLLOW_SMOOTH);
  tc->target = PLAYER;
}

void RegisterEntityData(world_t* w){
  RegisterPlayerData(w);
  RegisterWorldData(w);
  RegisterPrefabData(w);

}


