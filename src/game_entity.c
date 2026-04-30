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
  anim_player_t* a = InitAnimGroup("slime", SHEET_MOB);
  RegisterAnim(&world.anim, e, *a);

  Vector2 size = AnimSize(a);
  float radius = Vector2Length(size);

  rigid_body_t* rb = InitRigidBody(pos->vpos, SHAPE_CIRCLE, radius, 0.f);
  RigidBodyHasForce(rb, 8);
  force_t* bump = ForceBump(VEC_BOTH(0.25f));
  RigidBodySetPos(rb, pos->vpos);

  RigidBodyGiveForce(rb, bump);

  force_t* steer = ForceFromVec2(FORCE_STEERING, VEC_BOTH(0.175f));

  steer->max_velocity = 1.67f;
  steer->friction = VEC_BOTH(.47f);
  steer->threshold = 0.067f;
  RigidBodyGiveForce(rb, steer);

  RegisterRigidBody(&world.bodies, e, *rb);

  state_t* s = GameCalloc("InitState", 1, sizeof(state_t));
  RegisterState(&world.states, e, *s);
  behavior_t* bh = InitBehavior(8, 2, "ATTACK","WANDER");
  RegisterBehavior(&world.behaviors, e, *bh); 
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
void EntityAddPair(world_t* w, Entity e, uint32_t r, uint32_t o) {
    comp_id_t id = make_pair(r, o);
    component_pool_t* pool = get_or_create_pool(w, id);
    pool_add(pool, e);
}

Entity EntityPair(EntityManager* em, Entity r, Entity o){
  Entity e = EntityCreate(em);
  EntityAddPair(em, e, r.id, o.id);
}
