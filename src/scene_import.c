#include "scene_data.h"
#include "game_import.h"

void SceneImport(world_t* w, const char* path){
  for (int i = 0; i < NUM_PREFABS; i++){
    EntityPrefab prefab_data = PREFAB_DATA[i];

    Entity e = PrefabCreate(w, prefab_data.name);

    for(int j = 0; j < prefab_data.num_comp; j++)
      ImportPrefabComponent(w, e, prefab_data.components[j], path, prefab_data.name);
  }

  for(int i = 0; i < NUM_ENTS; i++){
    EntityInstance ent_data =  ENT_DATA[i];

    Vector2 pos = VEC_NEW(ent_data.x, ent_data.y);
    Entity e = PrefabSpawn(w, ent_data.prefab, pos);

    TraceLog(LOG_INFO,"==== SCENE IMPORT ===\n Spawn prefab %s as Ent %i at [%0.2f, %0.2f]", ent_data.prefab, e.id, pos.x, pos.y);

  }
}

void ImportPrefabComponent(world_t* w, Entity e, const char* comp, const char* path, const char* name){

  comp_id_t *cid = ComponentMapGetID(comp); 
  ComponentImportFn fn = ComponentMapFn(comp);

  if(cid && *cid < w->next_component_id){
    if(fn)
      fn(ComponentAdd(w, e, *cid), name);
    else
      ComponentAdd(w, e, *cid);
    
    TraceLog(LOG_INFO,"==== SCENE IMPORT ===\n added component %s %i to prefab %s", comp, *cid, name);
  }
}

void InputImport(void* c, const char* name){
  input_comp_t* ic = c;

  ic->input = *InitInput();
}


void CameraImport(void* c,const char* name){
  cam_comp_t* cc = c;

  cam_d data = GetCamData(name);

  Vector2 offset = VEC_NEW(data.offset_x, data.offset_y);
  cc->camera = *InitCamera(data.zoom, data.rot, offset);
}

void TrackingImport(void* c,const char* name){
  track_comp_t* tc = c;

  tc->ctx = *InitCameraContext(CAM_FOLLOW_SMOOTH);

}

void PhysicsImport(void* c,const char* name){
  phys_comp_t* pc = c;

  phys_d data = GetPhysData(name);

  pc->rb = *InitRigidBody(VECTOR2_ZERO, data.shape, data.wid, data.hei);

  for(int i = 0; i < FORCE_DONE; i++){
    force_d fdat = data.forces[i];
    if(fdat.type == FORCE_NONE)
      continue;

    force_t* f = ForceFromVec2(fdat.type, VEC_BOTH(fdat.speed));

    f->max_velocity = fdat.max_vel;
    f->friction = fdat.frict;

    f->threshold = fdat.threshold;

    RigidBodyGiveForce(&pc->rb, f);
  }
}


void SpriteImport(void*,const char*){

}

void AnimationImport(void* c, const char* name){
  anim_comp_t* ac = c;

  anim_d data = GetAnimData(name);
  ac->player.sheet_id = data.sheet;

  for(int s = 0; s < ANIM_DONE; s++)
    for(int i = 0; i < MAX_DIRECTIONS; i++){
      anim_seq_d seq_dat = data.sequences[s][i];
      if(seq_dat.state == ANIM_NONE)
        continue;
      ac->sequences[s][i] = *AnimRegisterState(data.sheet, name, seq_dat.name);
      ac->sequences[s][i].loop = seq_dat.loop;
      ac->sequences[s][i].on_end = seq_dat.on_end;
    }
}

void PositionImport(void* c,const char* name){
  pos_comp_t* pc = c;

  EntityInstance ent_data = GetEntData(name);

  pc->pos = *InitPosition(VEC_NEW(ent_data.x, ent_data.y));

}

void TypeImport(void* c,const char* name){
  type_comp_t* tc = c;

  EntityInstance ent_data = GetEntData(name);

  tc->type = ent_data.type;

}

