#include "game_systems.h"
#include "util_observe.h"

subject_t renderer;
bool RenderContextInit(void* comp, component_entry_t* j){
  render_ctx_t* r = comp;

  return ParseRenderComponent(j->data, r);
}

void RenderInit(world_t* w){
  SubjectInit(&renderer);
  window_resize_t resize = {GetScreenWidth(), GetScreenHeight()};
  SubjectNotify(&window, &resize);
}

void RenderLoad(world_t* w, Entity e){
}

void RenderBegin(world_t* w, Entity e){}

void RenderDraw(world_t* w, Entity e){
  render_ctx_t* ctx = GET_COMPONENT(w, e, render_ctx_t, RENDER_ID);

  SubjectNotify(&renderer, ctx);
}

void RenderEnd(world_t* w, Entity e){}
