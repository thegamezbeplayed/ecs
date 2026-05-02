#include "game_systems.h"

void RenderLoad(world_t* w, Entity e){

}

void RenderBegin(world_t* w, Entity e){
  cam_comp_t* c = GET_COMPONENT(w, e, cam_comp_t, CAM_ID);

  BeginDrawing();
  BeginTextureMode(c->view.tex);
  ClearBackground(BLACK);
  BeginMode2D(c->camera);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW);

}

void RenderEnd(world_t* w, Entity e){
  cam_comp_t* c = GET_COMPONENT(w, e, cam_comp_t, CAM_ID);
  EndMode2D();
  EndTextureMode();
  DrawTexturePro(c->view.tex.texture, c->view.view, c->view.bounds, VECTOR2_ZERO, 0, WHITE);

  EndDrawing();

}
