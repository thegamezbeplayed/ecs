#include "util_observe.h"
#include "system_define.h"

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
