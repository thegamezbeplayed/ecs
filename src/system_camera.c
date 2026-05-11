#include "game_systems.h"
#include "component_define.h"

void CameraLoad(world_t* w, Entity e){
  QueryBegin();
  comp_id_t seek[1];
  seek[0] = FOLLOW_ID;
  int count = QueryEntityByComp(w, 1, seek);
}

void CameraReady(world_t* w, Entity e){
  track_comp_t* t = GET_COMPONENT(w, e, track_comp_t, TRACK_ID);

  Entity tar = QueryGetNext(w);
  if(tar.id == INVALID_ENTITY.id)
    return;

  follow_comp_t* fc = GET_COMPONENT(w, tar, follow_comp_t, FOLLOW_ID);

  if(fc->assigned)
    return;

  t->target = tar.id;
  fc->assigned = true;
}

void CameraSystem(world_t* w, Entity e){
  cam_comp_t* c = GET_COMPONENT(w, e, cam_comp_t, CAM_ID);
  track_comp_t* t = GET_COMPONENT(w, e, track_comp_t, TRACK_ID);

  if(!t || !t->target)
    return;

  Entity tar = EntityGet(&w->manager, t->target);

  if(!EntityValid(&w->manager, tar))
    return;

  position_t* pos = ComponentGet(w, tar, POS_ID);
  
  if(!pos)
    return;

  TRACK(t->ctx.tracking, &c->camera, pos->vpos);

  Rectangle cropped_bounds = RECT_CROP(c->view.border, c->view.border_distance);

  Vector2 clamped = clamp_point_to_rect(c->camera.target, cropped_bounds);

  if(vec_compare(clamped, c->camera.target))
    return;

  c->camera.target = clamped;
}
