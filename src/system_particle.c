#include "game_systems.h"

static particle_layer_t PARTICLE_LAYERS[LAYER_DONE] = {0};

Entity ParticlePoolGetAvailable(RenderLayer l){
  for(int i = 0; i < MAX_PARTICLES; i++){
    if(PARTICLE_LAYERS[l].in_use[i])
      continue;

    PARTICLE_LAYERS[l].in_use[i] = true;
    return PARTICLE_LAYERS[l].ents[i];
  }

  return INVALID_ENTITY;
}

void ParticleRender(world_t* w, RenderLayer l){
  for(int i = 0; i < MAX_PARTICLES; i++){
    if(!PARTICLE_LAYERS[l].in_use[i])
      continue;

    Entity e = PARTICLE_LAYERS[l].ents[i];
    particle_t* pac = GET_COMPONENT(w, e, particle_t, PARTICLE_ID);
    position_t* poc = GET_COMPONENT(w, e, position_t, POS_ID);

    if(!pac || !pac->active){
      PARTICLE_LAYERS[l].in_use[i] = false;
      return;
    }
    switch(pac->draw_type){
      case PARTICLE_SPRITE:
        break;
      case PARTICLE_RECT:
        DrawRectangle(poc->vpos.x, poc->vpos.y, pac->wid, pac->hei, pac->color);
        break;
      case PARTICLE_CIRCLE:
        DrawCircleV(poc->vpos, pac->radius, pac->color);
        break;
      case PARTICLE_PIXEL:
        DrawPixelV(poc->vpos, pac->color);
        break;
    }
  }
}

void ParticleEmitEvent(event_t* ev, void* data){
  world_t* w = data;

  Entity e = EntityGet(&w->manager, ev->eid);

  switch(EVENT_ID(ev->type)){
    case PARTICLE_EVENT_START:
    case PARTICLE_EVENT_BURST:
      int count = ParticleEmit(w, e);
      break;
    case PARTICLE_EVENT_STOP:
      break;
    default:
      return;
      break;
  }
}

void ParticleOnRender(void* o_data, void* s, void* e_data) {
  render_ctx_t* r = e_data;

  world_t* w = o_data;

  ParticleRender(w, r->layer);
}

void ParticleEmitterLoad(world_t* w, Entity e){
  particle_emitter_t* ec = GET_COMPONENT(w, e, particle_emitter_t, EMITTER_ID);

  SubjectAddObserver(&renderer, ParticleOnRender, w);

  for( int i = 0; i < PARTICLE_EVENT_COUNT; i++){
    notification n = ParticleEvent_ToNotif(i);
    TargetSubscribe(n, ParticleEmitEvent, w, e.id);
  }

  Entity rel = EntityGetRelationTarget(w, e, REL_ChildOf);

  if(rel.id == INVALID_ENTITY.id)
    return;

  position_t* ep = GET_COMPONENT(w, e, position_t, POS_ID);
  position_t* rp = GET_COMPONENT(w, rel, position_t, POS_ID);

  PositionSet(ep, rp->vpos);
}

void ParticleEmitterSystem(world_t* w, Entity e){
  particle_emitter_t* ec = GET_COMPONENT(w, e, particle_emitter_t, EMITTER_ID);
  if(!ec->active)
    return;
  
  position_t* poc = GET_COMPONENT(w, e, position_t, POS_ID);

}

void ParticleSystem(world_t* w, Entity e){
  particle_t* pac = GET_COMPONENT(w, e, particle_t, PARTICLE_ID);
  position_t* poc = GET_COMPONENT(w, e, position_t, POS_ID);

  if(!pac->active)
    return;

  if(ParticleStep(pac, poc))
    return;

  pac->active = false;
}

void ParticlesInit(world_t* w){
  for(int i = 0; i < LAYER_DONE; i++)
    EntiyBatchReserve(&w->manager, MAX_PARTICLES, PARTICLE_LAYERS[i].ents);
}

void ParticleCleanup(world_t* w, Entity e){
  particle_t* pac = GET_COMPONENT(w, e, particle_t, PARTICLE_ID);

  if(pac->active)
    return;


  ComponentsClear(w, e);
}
