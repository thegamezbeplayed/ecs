#include "system_events.h"
#include "process_event.h"

static void ResolveBlock(world_t* w, Entity e, Entity tar)
{
  rigid_body_t* body = GET_COMPONENT(w, e, rigid_body_t, PHYS_ID);
  rigid_body_t* wall = GET_COMPONENT(w, tar, rigid_body_t, PHYS_ID);

  if (!body || !wall)
    return;

  Rectangle a = RigidBodyGetBoundsRec(body);
  Rectangle b = RigidBodyGetBoundsRec(wall);
  Rectangle overlap = GetCollisionRec(a, b);

  if (overlap.width <= 0 || overlap.height <= 0)
    return;

  Vector2 push = Vector2Zero();

  if (overlap.width < overlap.height) {
    push.x = (a.x + a.width * 0.5f < b.x + b.width * 0.5f)
      ? -overlap.width
      : overlap.width;
    body->vel.x = 0;
  } else {
    push.y = (a.y + a.height * 0.5f < b.y + b.height * 0.5f)
      ? -overlap.height
      : overlap.height;
    body->vel.y = 0;
  }

  body->bounds.pos = Vector2Add(body->bounds.pos, push);

  position_t* pos = GET_COMPONENT(w, e, position_t, POS_ID);
  if (pos) {
    pos->pos = body->bounds.pos;
    pos->next = VEC_UNSET;
  }
}

void PhysHandleEvent(world_t* w, Entity e, Entity tar , PhysicsEventID event){
  switch(event){
    //case PHYS_EVENT_COLL:
    case PHYS_EVENT_BLOCK:
      // stop or undo movement for e when it hits tar
      ResolveBlock(w, e, tar);
      break;
    case PHYS_EVENT_HIT:
      if(EntityHasRelation(w, e, "CollisionOf"))
        e = EntityGetRelationTarget(w, e, "CollisionOf");

      if(EntityHasRelation(w, tar, "CollisionOf"))
        return;

      GameEvent(PhysEvent_ToNotif(event), &tar, e.id);
      break;
  }
}
