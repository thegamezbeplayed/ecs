#include "game_register.h"
#include "game_systems.h"
void Draw2DGrid(int cellSize, int screenWidth, int screenHeight) {
  Color gridColor = GRAY;

  // Vertical lines
  for (int x = 0; x <= screenWidth; x += cellSize) {
    DrawLine(x, -160, x, screenHeight, gridColor);
  }

  // Horizontal lines
  for (int y = -160; y <= screenHeight; y += cellSize) {
    DrawLine(0, y, screenWidth, y, gridColor);
  }
}


void InitRenderSystem(render_st* r){
  r->tex = LoadRenderTexture(1600, 1200);

  RegisterScheduleStep(UPDATE_DRAW, RenderSystem);
}

void RenderSystem(system_pool_t* s, component_registry_t* c){
  render_st *r = &s->render;
  camera_c* cc = &c->cam;

  camera_t* cam = CameraGetActive(cc);
  if(!cam->camera)
    return;

  sprite_c* spr = &c->sprites;
  BeginDrawing();
  BeginTextureMode(r->tex);
  DrawRectangleLinesEx(cam->bounds, 1.5f, RED);
  BeginMode2D(*cam->camera);
  Draw2DGrid(16, GetScreenWidth()*4, GetScreenHeight()*4);

  ClearBackground(BLACK);
  for (uint32_t i = 0; i < spr->map.size; i++) {
    sprite_t* s = &spr->dense[i];

    DrawSprite(s);
  }
  
  EndMode2D();
  EndTextureMode();

  DrawRectangleLinesEx(cam->bounds, 1.5f, RED);
  DrawTexturePro(r->tex.texture, cam->view, cam->bounds, VECTOR2_ZERO, 0, WHITE);

  DrawFPS(10, 10);
  EndDrawing();

}

void CameraSync(system_pool_t* s, component_registry_t* c){
  camera_st* cs = &s->cam;
  sprite_c* spr = &c->sprites;
  camera_c* cam = &c->cam; 
  camera_follow_c* f = &c->view; 
  for(int i = 0; i < cam->map.size && f->map.size; i++){
    Entity e = f->map.entities[i];

    camera_t* ac = &cam->dense[i];
    if(!HasSprite(spr, e))
      continue;

    sprite_t* espr = SpriteGet(spr, e);
    ScreenCameraSync(ac, espr->pos);
  }
}

void InitCameraSystem(camera_st* c){
  c->active_camera = -1;

  RegisterScheduleStep(UPDATE_FRAME, CameraSync);
}
