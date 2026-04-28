#include "game_register.h"
#include "game_systems.h"

Vector2 ROOM_SIZE =  {1600,1200};

component_registry_t world;
system_pool_t run;
void TestComponents(void){

  Entity e = EntityCreate(&world.manager);

  PLAYER = e.id;
  position_t* p = InitPosition(CELL_NEW(4, 3));

  anim_player_t* a = InitAnimGroup("character", SHEET_CHAR);
 
  Vector2 size = AnimSize(a);

  float radius = Vector2Length(size);
  rigid_body_t* rb = InitRigidBody(p->vpos, SHAPE_CIRCLE, radius/4, 0.f);

  RigidBodyHasForce(rb, 16);
  Entity c = EntityCreate(&world.manager);
 
  force_t* bump = ForceBump(VEC_BOTH(0.25f));
  force_t* steer = ForceFromVec2(FORCE_STEERING, VEC_BOTH(0.25f));

  steer->friction = VEC_BOTH(.67f);
  steer->threshold = 0.067f;
  RigidBodyGiveForce(rb, bump);
  RigidBodyGiveForce(rb, steer);
  RigidBodySetPos(rb, p->vpos); 
  camera_t* cam = InitCamera(2.f, .0f, Vector2Scale(ROOM_SIZE,0.5f));

  input_t* gi = InitInput();
  camera_ctx_t* ctx = InitCameraContext(CAM_FOLLOW);
  RegisterInput(&world.input, e, *gi);
  RegisterCameraFollow(&world.view, e, *ctx);
  RegisterCamera(&world.cam, c, *cam);
  RegisterAnim(&world.anim, e, *a);
  RegisterPos(&world.pos, e, *p);
  RegisterRigidBody(&world.bodies, e, *rb);
  EntityTest(10);
}

void InitComponents(void){
  RegisterSystems();
  EntityInit(&world.manager);
  SpriteInit(&world.sprites);
  CameraInit(&world.cam);
  TestComponents();
}

uint32_t RegisterEntity(component_t* c, Entity e){
  uint32_t i = c->size++;

  c->entities[i] = e;
  c->sparse[e.id] = i;

  return i;
}

void RegisterRigidBody(rigid_body_c* c, Entity e, rigid_body_t b){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = b;
}

void RegisterPos(position_c* c, Entity e, position_t pos){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = pos;

}

void RegisterAnim(anim_c* c, Entity e, anim_player_t a){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = a;
  for (int j = 0; j < a.num_seq; j++)  
  RegisterAnimation(&run.anim, a.anims[j].group);

}

void RegisterCamera(camera_c* c, Entity e, camera_t s){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = s;
}

void RegisterCameraFollow(camera_follow_c* c, Entity e, camera_ctx_t s){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = s;

}

void RegisterInput(input_c* c, Entity e, input_t s){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = s;

}

void RegisterSprite(sprite_c* c, Entity e, sprite_t s){
  uint32_t i = RegisterEntity(&c->map, e);
  c->dense[i] = s;
}

bool HasSprite(sprite_c* c, Entity e) {
    int idx = c->map.sparse[e.id];

    return idx < c->map.size &&
           c->map.entities[idx].id == e.id;
}

void RegisterSystems(void){
  InitRenderSystem(&run.render);
  InitCameraSystem(&run.cam);
  InitInputSystem(&run.input);
  InitPositionSystem(&run.pos);
  InitAnimateSystem(&run.anim);
  InitPhysicsSystem(&run.phys);
}

void RegisterScheduleState(UpdateType u, SystemFn fn){

  run.schedule.states[u].tick[run.schedule.states[u].size++] = fn;
}

void RegisterScheduleStep(UpdateType u, SystemFn fn){

  run.schedule.steps[u].tick[run.schedule.steps[u].size++] = fn;
}

void SystemsState(GameState u){
  schedule_step_t* s = &run.schedule.states[u];

  for(int i = 0; i < s->size; i++)
    s->tick[i](&run, &world);

}

void SystemsStep(UpdateType u){
  schedule_step_t* s = &run.schedule.steps[u];

  for(int i = 0; i < s->size; i++)
    s->tick[i](&run, &world);
}
