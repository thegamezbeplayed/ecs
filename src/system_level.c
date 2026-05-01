#include "game_register.h"
#include "game_systems.h"

static choice_pool_t* locations;
void LevelLoad(world_t* w, Entity e){
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
}
