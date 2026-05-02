#include "game_systems.h"

void CameraSystem(world_t* w, Entity e){
  cam_comp_t* c = GET_COMPONENT(w, e, cam_comp_t, CAM_ID);
  track_comp_t* t = GET_COMPONENT(w, e, track_comp_t, TRACK_ID);

  Entity tar = EntityGet(&w->manager, t->target);

  if(!EntityValid(&w->manager, tar))
    return;

  pos_comp_t* pc = ComponentGet(w, tar, POS_ID);

  if(!pc)
    return;

  TRACK(t->ctx.tracking, &c->camera, pc->pos.vpos);

  Rectangle cropped_bounds = RECT_CROP(c->view.border, c->view.border_distance);

  Vector2 clamped = clamp_point_to_rect(c->camera.target, cropped_bounds);

  if(vec_compare(clamped, c->camera.target))
    return;

  c->camera.target = clamped;
}
