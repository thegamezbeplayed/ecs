#include "game_systems.h"

static bool clear = true;

void RenderLoad(world_t* w, Entity e){

}

void RenderBegin(world_t* w, Entity e){
  view_comp_t* vc = GET_COMPONENT(w, e, view_comp_t, VIEW_ID);
}

void RenderDraw(world_t* w, Entity e){
  view_comp_t* vc = GET_COMPONENT(w, e, view_comp_t, VIEW_ID);
  BeginTextureMode(vc->view.tex);
  ClearBackground(BLANK);

  notification n = ViewEvent_ToNotif(VIEW_EVENT_DRAW);
  GameEvent(n, vc, vc->layer);
  EndTextureMode();
  DrawTexturePro(vc->view.tex.texture, vc->view.view, vc->view.bounds, vc->view.origin, 0, WHITE);
}

void RenderEnd(world_t* w, Entity e){
  view_comp_t* vc = GET_COMPONENT(w, e, view_comp_t, VIEW_ID);
}
