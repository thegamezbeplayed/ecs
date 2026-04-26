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
  position_c* pos = &c->pos;

  EntityManager *em = &c->manager;
  camera_t* cam = CameraGetActive(cc);
  if(!cam->camera)
    return;

  sprite_c* spr = &c->sprites;
  anim_c* a = &c->anim;
  BeginDrawing();
  BeginTextureMode(r->tex);
  DrawRectangleLinesEx(cam->bounds, 1.5f, RED);
  BeginMode2D(*cam->camera);
  ClearBackground(BLACK);
  Draw2DGrid(16, GetScreenWidth()*4, GetScreenHeight()*4);

  for(int i = 0; i < a->map.size; i++){
    anim_player_t* ap = &a->dense[i];
    Entity* e = ComponentGetEntity(em, &a->map, i);
    int pidx = ComponentByEntity(&pos->map, e->id);
    position_t* p = &pos->dense[pidx];

    sprite_slice_t* frame = AnimGetFrame(ap);
    DrawSlice(frame, p->vpos, 0);

  }

  for (uint32_t i = 0; i < spr->map.size; i++) {
    sprite_t* s = &spr->dense[i];
    Entity* e = ComponentGetEntity(em, &spr->map, i);
    if(!e)
      continue;

    int pidx = ComponentByEntity(&pos->map, e->id);
    position_t* p = &pos->dense[pidx];

    DrawSpriteAtPos(s, p->vpos);
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
  camera_c* cam = &c->cam; 
  camera_follow_c* f = &c->view; 
  position_c* pos = &c->pos;


  for(int i = 0; i < cam->map.size && f->map.size; i++){
    position_t* p = &pos->dense[i];
    camera_t* ac = &cam->dense[i];

    ScreenCameraSync(ac, p->vpos);
  }
}

void InitCameraSystem(camera_st* c){
  c->active_camera = -1;

  RegisterScheduleStep(UPDATE_FRAME, CameraSync);
}
