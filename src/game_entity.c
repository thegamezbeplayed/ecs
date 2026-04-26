#include "game_register.h"

void EntityInit(EntityManager* em) {
  em->free_count = MAX_ENTITIES;

  for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
    em->free_list[i] = MAX_ENTITIES - i - 1;
    em->generation[i] = 0;
    em->alive[i] = 0;
  }
}

Entity EntityCreate(EntityManager* em) {
  uint32_t id = em->free_list[--em->free_count];

  em->alive[id] = 1;

  Entity e = {
    .id = id,
    .generation = em->generation[id]
  };

  return e;
}

void EntityDestroy(EntityManager* em, Entity e) {
  if (!em->alive[e.id]) return;

  em->alive[e.id] = 0;

  em->generation[e.id]++;   // invalidate old references

  em->free_list[em->free_count++] = e.id;
}

bool EntityValid(EntityManager* em, Entity e) {
  return em->alive[e.id] &&
    em->generation[e.id] == e.generation;
}

void EntityTestSpawn(Cell c){
  Entity e = EntityCreate(&world.manager);

  position_t *pos = InitPosition(c);
  RegisterPos(&world.pos, e, *pos);
  sprite_t* s = InitSpriteByTag("character", SHEET_CHAR);
  s->is_visible = true;
  RegisterSprite(&world.sprites, e, *s);
}

void EntityTest(int count){
  choice_pool_t* cp = InitChoicePool(4*count*count, ChooseByWeight);

  for (int x = -count; x < count; x++){
    for (int y = -count; y < count; y++){
      int score = abs(x+y);
      Cell pos = CELL_NEW(x,y);
      int id = IntGridIndex(x,y);
      AddChoice(cp, id, score, &pos, DiscardChoice);
    }
  }

  ShuffleChoices(cp);
  while(count > 0){
    choice_t* c = cp->choose(cp);

    Cell pos = GridIndexCell(c->id);
    EntityTestSpawn(pos);
    count--;

  }
}
