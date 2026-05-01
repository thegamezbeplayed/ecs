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

int PHYS_SYS;

void RegisterComponentData(world_t* w) {
  ANIM_ID = REGISTER_COMPONENT(w, anim_comp_t);
  NAME_ID = REGISTER_COMPONENT(w, name_comp_t);
  POS_ID = REGISTER_COMPONENT(w, pos_comp_t);
  INPUT_ID = REGISTER_COMPONENT(w, input_comp_t);
  PHYS_ID = REGISTER_COMPONENT(w, phys_comp_t);//DuplicateRigidBody);
  LVL_ID = REGISTER_COMPONENT(w, lvl_comp_t);
  CAM_ID = REGISTER_COMPONENT(w, cam_comp_t);
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

  lset[GAME_READY] = LevelLoad;

  system_t* lvlsys = SystemRegister(w, ltick, lset);
  SystemRequire(lvlsys, LVL_ID);

  SystemCB rntick[UPDATE_DONE] = {0};
  rntick[UPDATE_DRAW_BEGIN] = RenderBegin;
  rntick[UPDATE_DRAW_END] = RenderEnd;

  SystemCB rnset[GAME_DONE] = {0};
  rnset[GAME_READY] = RenderLoad;

  system_t* rnsys = SystemRegister(w, rntick, rnset);

  SystemRequire(rnsys, CAM_ID);



}
void RegisterPlayerData(world_t* w){
  Entity l = EntityCreate(&w->manager);
  ComponentAdd(w, l, LVL_ID);

  Entity c = EntityCreate(&w->manager);
  ComponentAdd(w, c, CAM_ID);


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

  steer->max_velocity = 1.67f;
  steer->friction = VEC_BOTH(.47f);
  steer->threshold = 0.067f;
  RigidBodyGiveForce(&phc->rb, steer);

  name_comp_t* nc = ComponentAdd(w, p, NAME_ID);

  strncpy(nc->name, "Guy", MAX_NAME_LEN);
  nc->name[MAX_NAME_LEN - 1] = '\0';

}

void RegisterEntityData(world_t* w){
  RegisterPlayerData(w);

  PrefabRegistryInit(w);

  Entity slime = PrefabCreate(w, "Slime");

  anim_comp_t* ac = ComponentAdd(w, slime, ANIM_ID);
  ac->player.sheet_id = SHEET_MOB;
  
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

}
