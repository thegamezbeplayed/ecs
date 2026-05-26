#include "util_observe.h"
#include "system_define.h"
#include "tool_lookup.h"

void ObserveInit(world_t* w){
  InitSubjectPool(MAX_ENTITIES);

  if(SUBJECTS.num_store == 0)
    return;

  hash_iter_t iter;
  HashStart(&SUBJECTS.stored, &iter);

  hash_slot_t* s;
  while((s = HashNext(&iter))){
    subject_store_t* store = s->value;

    TraceLog(LOG_INFO, "=== OBSERVE INIT ===\n NOTIFY %s", store->subject->name);
    SubjectRunNotify(store->subject, store->data, -1, INVALID_NOTIF);

    HashPut(&SUBJECTS.map, store->key, store->subject);
  }

  HashFree(&SUBJECTS.stored);
  SUBJECTS.num_store = 0;
}

void ObserveReady(world_t* w, Entity e){
  component_observer_t* c = GET_COMPONENT(w, e, component_observer_t, OBSERVE_ID);

  Entity o = e;
  if(EntityHasRelation(w, e, REL_Observes))
    o = EntityGetRelationTarget(w, e, REL_Observes);

  comp_id_t s_cid = c->relation;
  for(int i = 0; i < c->num_subj; i++){
    for (int j = 0; j < c->num_obs; j++){
      comp_id_t l_cid = ComponentGetID(c->observers[j]);
      if(!ComponentValid(w, l_cid)){
        TraceLog(LOG_WARNING, "=== OBSERVER READY ===\n Invalid component %s",c->observers[j]);
        continue;
      }
      ObserverCB cb = LookupSystemSink(c->observers[j]);
      if(!cb){
        TraceLog(LOG_WARNING, "=== OBSERVER READY ===\n No Observer Callback %s",c->observers[j]);
        continue;
      }

      switch(c->type){
        case OBS_COMP:
        case OBS_ENT:
          SubjectAddObserverByComponent(c->subjects[i], o.id, s_cid, c->name, cb,  ComponentGet(w, o, l_cid));
          break;
        case OBS_DEFINE:
          SubjectAddObserver(c->subjects[i], c->name, cb, ComponentGet(w, o, l_cid));
          break;
      }
    }
  }
}

void SubjectLoad(world_t* w, Entity e){
  subject_component_t* sc = GET_COMPONENT(w, e, subject_component_t, SUBJECT_ID);

  Entity rel = EntityGetRelationTarget(w, e, REL_SubjectOf);

  sc->key = SubjectComponent(sc->name, rel.id, sc->comp);
}

void SubjectSystem(world_t* w, Entity e){
  subject_component_t* sc = GET_COMPONENT(w, e, subject_component_t, SUBJECT_ID);

  Entity rel = EntityGetRelationTarget(w, e, REL_SubjectOf);

  if(!ComponentCheck(w, sc->comp, rel))
    return;  

  subject_t* s = SubjectGetByKey(sc->key);
  SubjectRunNotify(s, ComponentGet(w, rel, sc->comp), sc->comp, sc->event);

  sc->ran = true;
}

void SubjectCleanup(world_t* w, Entity e){
  subject_component_t* sc = GET_COMPONENT(w, e, subject_component_t, SUBJECT_ID);

  if(!sc->ran)
    return;

  sc->ran = false;

  Entity rel = EntityGetRelationTarget(w, e, REL_SubjectOf);
  ComponentClearUpdate(w, rel, sc->comp);
}
