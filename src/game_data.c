#include "game_define.h"
#include "game_systems.h"

uint64_t ANIM_ID;
uint64_t NAME_ID;
uint64_t POS_ID;
uint64_t INPUT_ID;
uint64_t PHYS_ID;

int PHYS_SYS;

void RegisterComponentData(world_t* w) {
  ANIM_ID = REGISTER_COMPONENT(w, sizeof(anim_comp_t));
  NAME_ID = REGISTER_COMPONENT(w, sizeof(name_comp_t));
  POS_ID = REGISTER_COMPONENT(w, sizeof(pos_comp_t));
  INPUT_ID = REGISTER_COMPONENT(w, sizeof(input_comp_t));
  PHYS_ID = REGISTER_COMPONENT(w, sizeof(phys_comp_t));

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

}

void RegisterEntityData(world_t* w){
  Entity p = EntityCreate(&w->manager);
  PLAYER = p.id;

  anim_comp_t* ac = ComponentAdd(w, p, ANIM_ID);

  pos_comp_t* pc = ComponentAdd(w, p, POS_ID);
  pc->pos = InitPosition(CELL_NEW(15,10));
  ac->player = InitAnimGroup("player", SHEET_CHAR);

  input_comp_t* ic = ComponentAdd(w, p, INPUT_ID);
  ic->input = InitInput();

  phys_comp_t* phc = ComponentAdd(w, p, PHYS_ID);

  Vector2 size = AnimSize(ac->player);
  phc->rb = InitRigidBody(pc->pos->vpos, SHAPE_CIRCLE, size.x, size.y);
  
  RigidBodyHasForce(phc->rb, 8);
  force_t* bump = ForceBump(VEC_BOTH(0.25f));

  RigidBodyGiveForce(phc->rb, bump);

  force_t* steer = ForceFromVec2(FORCE_STEERING, VEC_BOTH(0.175f));

  steer->max_velocity = 1.67f;
  steer->friction = VEC_BOTH(.47f);
  steer->threshold = 0.067f;
  RigidBodyGiveForce(phc->rb, steer);

  name_comp_t* nc = ComponentAdd(w, p, NAME_ID);

  strncpy(nc->name, "Guy", MAX_NAME_LEN);
  nc->name[MAX_NAME_LEN - 1] = '\0';
}
