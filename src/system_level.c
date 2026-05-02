#include "game_register.h"
#include "game_systems.h"

static choice_pool_t* locations;

void LevelLoad(world_t* w, Entity e){
  lvl_comp_t* l = GET_COMPONENT(w, e, lvl_comp_t, LVL_ID);

  l->bounds = Rect(0, 0, l->wid, l->hei);

  prefab_registry_t* p = &w->prefabs;

  int num_objs =  0;
  prefab_t objs[4] = {0};

  for(int i = 0; i < p->count; i++){
    prefab_t pspawn = p->prefabs[i];

    type_comp_t* t = ComponentGet(w,pspawn.entity, TYPE_ID);
    if(t->type != ENT_OBJ)
      continue;

    objs[num_objs++] = pspawn;
  }
 
  if(num_objs == 0)
   return;


  PrefabSpawn(w, objs[0].name, VEC_NEW(0,0));
  PrefabSpawn(w, objs[0].name, VEC_NEW(0,l->hei));
  PrefabSpawn(w, objs[0].name, VEC_NEW(l->hei,0));
  PrefabSpawn(w, objs[0].name, VEC_NEW(l->hei,l->wid));

}

void LevelReady(world_t* w, Entity e){
  prefab_registry_t* p = &w->prefabs;

  Vector2 origin = VECTOR2_ZERO;
  bool ready = false;
  locations = StartChoice(&locations, MOB_MAX*MOB_MAX, ChooseByWeight, &ready);
  if(!ready){
    for(int x = 0; x < MOB_MAX; x++)
      for(int y = 0; y < MOB_MAX; y++){
        Vector2 *pos = GameCalloc("LevelLoad", 1, sizeof(Vector2));
        *pos = VEC_NEW(x*CELL_WIDTH/3, y* CELL_WIDTH/4 );

        int score = Vector2Distance(*pos, origin);

        int id = IntGridIndex(pos->x, pos->y);
        AddChoice(locations, id, score, pos, DiscardChoice);

      }

    ShuffleChoices(locations);
  }

  for(int i = 0; i < p->count; i++){
    prefab_t* pspawn = &p->prefabs[i];

    type_comp_t* type = ComponentGet(w,pspawn->entity, TYPE_ID);
    if(!type || type->type != ENT_MOB)
      continue;

    int count = 0;
    for(int j = 0; j < MOB_MAX; j++){
      choice_t* c = locations->choose(locations);

      if(!c || !c->context)
        continue;

      count++;
      Vector2 pos = *(Vector2*)c->context;
      PrefabSpawn(w, pspawn->name, pos);
    }

    TraceLog(LOG_INFO,"=== LEVEL LOAD ===\n Created %i %s", count, p->prefabs[0].name);
  }

}

void LevelSystem(world_t* w, Entity e){
}

void LevelRender(world_t* w, Entity e){
  lvl_comp_t* l = GET_COMPONENT(w, e, lvl_comp_t, LVL_ID);

  DrawRectangleLinesEx(l->bounds, 1.5f, RED);


}
