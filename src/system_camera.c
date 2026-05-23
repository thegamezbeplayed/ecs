#include "component_define.h"
#include "system_define.h"

void CameraOnWindowResize(void* o_data, void* s, payload_t* p) {
  window_resize_t* w = p->data;
  camera_t* c = o_data;

  Vector2 offset = VEC_SCALE(VEC_NEW(w->width, w->height), 0.5);
  CameraSetOffset(c, offset);
}

void CameraLoad(world_t* w, Entity e){
  camera_t* c = GET_COMPONENT(w, e, camera_t, CAM_ID);
  SubjectAddObserver("WINDOW", "CAMERA", CameraOnWindowResize, c);

  int wid = GetScreenWidth();
  int hei = GetScreenHeight();
  Vector2 offset = VEC_SCALE(VEC_NEW(wid, hei), 0.5);
  CameraSetOffset(c, offset);

}

void CameraReady(world_t* w, Entity e){
  QueryBegin();
  comp_id_t seek[1];
  seek[0] = FOLLOW_ID;
  int count = QueryEntityByComp(w, 1, seek);
}

void CameraTarget(world_t* w, Entity e){
  tracking_t* t = GET_COMPONENT(w, e, tracking_t, TRACK_ID);

  Entity tar = QueryGetNext(w);
  if(tar.id == INVALID_ENTITY.id)
    return;

  follow_t* f = GET_COMPONENT(w, tar, follow_t, FOLLOW_ID);

  if(f->assigned)
    return;

  t->target = tar.id;
  f->assigned = true;
}

void CameraSystem(world_t* w, Entity e){
  camera_t* c = GET_COMPONENT(w, e, camera_t, CAM_ID);
  tracking_t* t = GET_COMPONENT(w, e, tracking_t, TRACK_ID);

  if(!t || !t->target)
    return;

  Entity tar = EntityGet(&w->manager, t->target);

  if(!EntityValid(&w->manager, tar))
    return;

  position_t* pos = ComponentGet(w, tar, POS_ID);
  
  if(!pos)
    return;

  TRACK(t->ctx.tracking, c, pos->pos);
}

void CameraBegin(world_t* w, Entity e){
  camera_t* c = GET_COMPONENT(w, e, camera_t, CAM_ID);
  BeginDrawing();
  BeginMode2D(*c);
  ClearBackground(BLANK);
}

void CameraEnd(world_t* w, Entity e){
  camera_t* c = GET_COMPONENT(w, e, camera_t, CAM_ID);
  EndMode2D();
  DrawFPS(10,10);
  EndDrawing();
}

