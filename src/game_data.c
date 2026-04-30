#include "game_define.h"
#include "game_systems.h"

uint64_t ANIM_ID;
uint64_t NAME_ID;
uint64_t POS_ID;

void RegisterComponentData(world_t* w) {
  ANIM_ID = REGISTER_COMPONENT(w, sizeof(anim_component_t));
  NAME_ID = REGISTER_COMPONENT(w, sizeof(name_component_t));
  POS_ID = REGISTER_COMPONENT(w, sizeof(pos_component_t));
}


void RegisterSystemData(world_t* w){
  SystemCB atick[UPDATE_DONE] = {0};
  atick[UPDATE_FIXED] = AnimSystem;
  atick[UPDATE_DRAW] = AnimRender;

  SystemCB aset[GAME_DONE] = {0};

  aset[GAME_READY] = AnimSystem;

  system_t* asys = SystemRegister(w, atick, aset);

  SystemRequire(asys, ANIM_ID);
  SystemRequire(asys, POS_ID);

}

void RegisterEntityData(world_t* w){
  Entity p = EntityCreate(&w->manager);
  PLAYER = p.id;

  anim_component_t* ac = ComponentAdd(w, p, ANIM_ID);

  pos_component_t* pc = ComponentAdd(w, p, POS_ID);
  pc->pos = InitPosition(CELL_NEW(15,10));
  ac->player = InitAnimGroup("player", SHEET_CHAR);

  name_component_t* nc = ComponentAdd(w, p, NAME_ID);

  strncpy(nc->name, "Guy", MAX_NAME_LEN);
  nc->name[MAX_NAME_LEN - 1] = '\0';
}
