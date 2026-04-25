#include "game_register.h"
#include "game_systems.h"

Vector2 ROOM_SIZE =  {1600,1200};

component_registry_t world;
system_pool_t run;
void TestComponents(void){

  Entity e = EntityCreate(&world.manager);

  sprite_t* s = InitSpriteByID(CHAR_PLAYER, SHEET_CHAR);
  s->pos = Vector2XY(500, 200);
  s->is_visible = true;
  RegisterSprite(&world.sprites, e, *s);
  Entity c = EntityCreate(&world.manager);
  
  camera_t* cam = InitCamera(2.f, .0f, Vector2Scale(ROOM_SIZE,0.5f));

  input_t* gi = InitInput();
  camera_ctx_t* ctx = InitCameraContext(CAM_FOLLOW);
  RegisterInput(&world.input, e, *gi);
  RegisterCameraFollow(&world.view, e, *ctx);
  RegisterCamera(&world.cam, c, *cam);

  EntityTest(100);
}

void InitComponents(void){
  EntityInit(&world.manager);
  SpriteInit(&world.sprites);
  CameraInit(&world.cam);
  TestComponents();
  RegisterSystems();
}

uint32_t RegisterEntity(component_t* c, Entity e){
  uint32_t i = c->size++;

  c->entities[i] = e;
  c->sparse[e.id] = i;

  return i;
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
}

void RegisterScheduleStep(UpdateType u, SystemFn fn){

  run.schedule.steps[u].tick[run.schedule.steps[u].size++] = fn;
}

void SystemsStep(UpdateType u){
  schedule_step_t* s = &run.schedule.steps[u];

  for(int i = 0; i < s->size; i++)
    s->tick[i](&run, &world);
}
