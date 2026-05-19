#include "util_observe.h"
#include "system_define.h"

void ObserverTest(void* obs_data, void* sub, void* ev_data){
  TraceLog(LOG_INFO, "OBSERVER TEST SUCCESS!");
}

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
    SubjectRunNotify(store->subject, store->data);

    HashPut(&SUBJECTS.map, store->key, store->subject);
  }

  HashFree(&SUBJECTS.stored);
  SUBJECTS.num_store = 0;
}

void ObserveReady(world_t* w, Entity e){
  component_observer_t* c = GET_COMPONENT(w, e, component_observer_t, OBSERVE_ID);

  for(int i = 0; i < c->num_subj; i++){
    for (int j = 0; i < MAX_LISTENERS; i++){
      comp_id_t cid = c->observers[j];
      if(!ComponentValid(w, cid))
        continue;

      SubjectAddObserverByComponent(c->subjects[i], e.id, cid, c->name,
          ObserverTest, ComponentGet(w, e, cid));

    }
  }
}
