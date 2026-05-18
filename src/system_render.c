#include "system_define.h"
#include "util_observe.h"

bool RenderContextInit(void* comp, component_entry_t* j){
  render_ctx_t* r = comp;

  return ParseRenderComponent(j->data, r);
}

void RenderInit(world_t* w){
}

void RenderLoad(world_t* w, Entity e){
  render_ctx_t* ctx = GET_COMPONENT(w, e, render_ctx_t, RENDER_ID);

  SubjectRegister(ctx->name);
}

void RenderBegin(world_t* w, Entity e){}

void RenderDraw(world_t* w, Entity e){
  render_ctx_t* ctx = GET_COMPONENT(w, e, render_ctx_t, RENDER_ID);

  SubjectNotify(ctx->name, ctx);
}

void RenderEnd(world_t* w, Entity e){}
