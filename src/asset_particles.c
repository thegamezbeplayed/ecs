#include "game_define.h"
#include "component_define.h"

bool ParticleInit(void* comp, component_entry_t* e){


}

bool ParticleEmitterInit(void* comp, component_entry_t* e){
  particle_emitter_t *p = comp;

  return ParseParticleEmitterComponent(e->data, p);
}

Entity ParticleSpawn(world_t* w, const particle_emitter_t* ec, Vector2 origin){
  Entity p = ParticlePoolGetAvailable(ec->layer);
  if(p.id == INVALID_ENTITY.id)
    return p;

  particle_t* pc = ComponentAdd(w, p, PARTICLE_ID);
  position_t* ps = ComponentAdd(w, p, POS_ID);

  pc->draw_type = ec->draw_type;
  pc->layer = ec->layer;
  
  pc->sheet_id = ec->sheet_id;
  pc->sheet_index = ec->sheet_index;

  Vector2 start = Vector2Subtract(origin, ec->spawn_radius);
  Vector2 end = Vector2Add(origin, ec->spawn_radius);
  PositionSet(ps, rand_range_vec(start, end));
  pc->dur = rand_range_int(ec->dur_min, ec->dur_max);

  pc->scale = pc->start_scale = ec->scale_min;
  pc->end_scale = ec->scale_end;
  pc->color = pc->start_color = ec->start_color;
  pc->end_color = ec->end_color;
  
  pc->accel = ec->acceleration;

  switch(pc->draw_type){
    case PARTICLE_SPRITE:
      break;
    case PARTICLE_RECT:
      pc->wid = ec->wid;
      pc->hei = ec->hei;
      break;
    case PARTICLE_CIRCLE:
      pc->radius = rand_range_float(ec->wid, ec->hei);
      break;
    case PARTICLE_PIXEL:
      break;
  }
  pc->active = pc->dur > 0;
  return p;
}

int ParticleEmit(world_t* w, Entity e){
  particle_emitter_t* ec = GET_COMPONENT(w, e, particle_emitter_t, EMITTER_ID);
  position_t* poc = GET_COMPONENT(w, e, position_t, POS_ID);

  Vector2 pos = Vector2Add(poc->vpos, ec->offset);
  int count = rand_range_int(1, ec->max);
  int spawned = 0;
  for(int i = 0; i < count; i++){
    Entity spawn = ParticleSpawn(w, ec, pos);
    if(spawn.id == INVALID_ENTITY.id){
      TraceLog(LOG_WARNING, "=== PARTICLE EMIT ===\n Layer %i max particles reached!", ec->layer);
      break;
    }
    spawned++;
  }

  return spawned;
}

bool ParticleStep(particle_t* pa, position_t* pos){
  if(pa->age >= pa->dur)
    return false;

  float rate = 1.f / pa->dur;

  pa->color = ColorLerp(pa->color, pa->end_color, rate);

  pa->vel = Vector2Add(pa->accel, pa->vel);
  
  pa->vel = Vector2Scale(pa->vel, 1 - pa->drag);
  PositionAddStep(pos, pa->vel);

  pa->age++;
  return true;
}
