#include "scene_data.h"
#include "game_import.h"
#include "game_helpers.h"

void SceneImport(world_t* w, const char* path){
  for (int i = 0; i < NUM_PREFABS; i++){
    EntityPrefab prefab_data = PREFAB_DATA[i];

    Entity e = PrefabCreate(w, prefab_data.name);
    for(int j = 0; j < prefab_data.num_comp; j++) {
      ImportPrefabComponent(w, e, prefab_data.components[j], path, prefab_data.name);
    }
  }
  for(int i = 0; i < NUM_ENTS; i++){
    EntityInstance ent_data =  ENT_DATA[i];

    Vector2 pos = VEC_NEW(ent_data.x, ent_data.y);
    Entity e = PrefabSpawn(w, ent_data.prefab, pos);

    EntityRelations erelates = GetRelationData(ent_data.prefab);
    for(int i = 0; i < erelates.count; i++){
      ImportPrefabRelation(w, e, ent_data.prefab, erelates.comps[i]);
    }
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

  }
}

void ImportPrefabRelation(world_t* w, Entity e, const char* ename, RelationComp comp){
  comp_id_t *cid = ComponentMapGetID(comp.comp);
  ComponentImportFn fn = ComponentMapFn(comp.comp);

  if(!cid){
    TraceLog(LOG_WARNING,"=== SCENE IMPORT ===\n Pair %s Component %s not found", ename, comp.comp);
    return;
  }

  for(int i = 0; i < comp.count; i++){
    RelationPair pair = comp.pairs[i];
    Entity rel = EntityCreate(&w->manager);

    if(fn)
      fn(ComponentAdd(w, rel, *cid), pair.name);

    EntityAddRelation(w, rel, pair.type, e);
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

  Vector2 size = VEC_NEW(data.wid, data.hei);

  Rectangle bounds = RECT(data.bx, data.by, data.bw, data.bh);
  cc->view = *InitView(size, bounds, 0);
}

void TrackingImport(void* c,const char* name){
  track_comp_t* tc = c;

  tc->ctx = *InitCameraContext(CAM_FOLLOW_SMOOTH);

}

void PhysicsImport(void* c,const char* name){
  phys_comp_t* pc = c;

  phys_d data = GetPhysData(name);

  pc->rb = *InitRigidBody(VECTOR2_ZERO, data.shape, data.wid, data.hei);

}

void ForceImport(void* c,const char* name){
  force_comp_t* fc = c;

  force_d fdat = GetForceData(name);

  fc->f = *ForceFromVec2(fdat.type, VEC_BOTH(fdat.speed));

  fc->f.speed = fdat.speed;
  fc->f.max_velocity = fdat.max_vel;
  fc->f.friction = fdat.frict;
  fc->f.event = fdat.event;
  fc->f.react = fdat.react;
  fc->f.threshold = fdat.threshold;

}

void SpriteImport(void*,const char*){

}

void AnimationImport(void* c, const char* name){
  anim_comp_t* ac = c;

  anim_d data = GetAnimData(name);
  ac->player.sheet_id = data.sheet;

  int hurt_count = 0;
  int HURT_FRAMES[MAX_SPRITE_FRAMES] = {0};
  for(int i = 0; i < MAX_SLICES; i++){
    collision_d cdata = SHEETS[data.sheet].coll[i];

    switch(cdata.type){
      case COL_HURT:
        HURT_FRAMES[hurt_count++] = cdata.frame;
        //ac->on_coll_frame[ac->num_hurt] = AnimCollisionHurt;
        ac->hurtboxes[ac->num_hurt++] = cdata;
        break;
      case COL_HIT:
        ac->hitbox = cdata;
        break;
    }
  }

  for(int s = 0; s < ANIM_DONE; s++)
    for(int i = 0; i < MAX_DIRECTIONS; i++){
      anim_seq_d seq_dat = data.sequences[s][i];
      if(seq_dat.state == ANIM_NONE)
        continue;
      AnimState ast = seq_dat.state;
      anim_t* a = AnimRegisterState(data.sheet, name, seq_dat.name);
      if(!a)
        continue;

      a->interupt = (ast < ANIM_ATTACK);
      for (int j = 0; j < a->count; j++){
       if(!array_contains_int(HURT_FRAMES, hurt_count, a->frames[j]))
        continue;

        a->hurtbox_index = j;

        a->on_frame_start[j] = ANIM_HURTBOX;
      }
      ac->sequences[ast][i] = *a;
      ac->sequences[ast][i].loop = seq_dat.loop;
      ac->sequences[ast][i].on_end = seq_dat.end;
    }
  
  ac->player.state = ANIM_IDLE;
  ac->player.dir = 3;
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

void StatImport(void* c,const char* name){
  stat_comp_t* sc = c;

  stat_d stat = GetStatData(name);

  sc->stat = *InitStat(stat.type, stat.min, stat.max, stat.val);
  sc->stat.on_empty = stat.on_empty;
  sc->stat.on_full = stat.on_full;
}
