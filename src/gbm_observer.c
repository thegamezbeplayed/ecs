#include "util_observe.h"
#include "game_define.h"
#include "components.h"
subject_pool_t SUBJECTS;

bool ObserverInit(void* comp, component_entry_t* data){

}

subject_t* SubjectCheckExists(const char* name, const char* from){
  hash_key_t hash = hash_str_64(name);
  if(!SUBJECTS.initialized){
    subject_store_t* store = HashGet(&SUBJECTS.stored, hash);
    if(!store)
      return NULL;

    return store->subject;
  }
  subject_t* s = HashGet(&SUBJECTS.map, hash);
  if(!s)
    return NULL;

  return s;
}


subject_t* SubjectGetEntry(const char* name, const char* from){
  hash_key_t hash = hash_str_64(name);
  if(!SUBJECTS.initialized){
    subject_store_t* store = HashGet(&SUBJECTS.stored, hash);
    if(!store)
      return NULL;

    return store->subject;
  }
  subject_t* s = HashGet(&SUBJECTS.map, hash);
  if(!s){
    TraceLog(LOG_WARNING, "=== SUBJECT %s ===\n Subject %s not found", from, name);
    return NULL;
  }

  return s;
}

void InitSubjectPool(int cap){
  HashInit(&SUBJECTS.map, next_pow2_int(cap));

  SUBJECTS.initialized = true;
}

void SubjectStore(const char* name, void* data){
  if(SUBJECTS.cap == 0){
    SUBJECTS.cap = MAX_SUBJECT_STORE;
    HashInit(&SUBJECTS.stored, MAX_SUBJECT_STORE);
  }

  subject_store_t* store = GameCalloc("SubjectStore", 1, sizeof(subject_store_t));
  hash_key_t hash = hash_str_64(name);

  store->subject = GameCalloc("SubjectStore", 1, sizeof(subject_t));
  strcpy(store->subject->name, name);
  store->key = hash;
  store->data = data;
  HashPut(&SUBJECTS.stored, hash, store);
  SUBJECTS.num_store++;
}

subject_t* SubjectComponent(const char* name, uint32_t eid, comp_id_t compid){
  uint64_t name_hash = hash_64_combine(hash_str_32(name), eid);
  hash_key_t key = hash_combine_64(name_hash, compid);

  subject_t* existing = HashGet(&SUBJECTS.map, key);
  if(existing)
    return existing;

   subject_t* s = GameCalloc("SubjectRegister", 1, sizeof(subject_t));
  strcpy(s->name, name);

  HashPut(&SUBJECTS.map, key, s);
}

subject_t* SubjectRegister(const char* name){
  subject_t* existing = SubjectCheckExists(name, "REGISTER");
  if(existing)
    return existing;

  hash_key_t hash = hash_str_64(name);
  subject_t* s = GameCalloc("SubjectRegister", 1, sizeof(subject_t));
  strcpy(s->name, name);
  HashPut(&SUBJECTS.map, hash, s);

  return s;
}

void SubjectAddObserverByComponent(const char* name, uint32_t eid, comp_id_t compid, const char* oname, ObserverCB cb, void* data){
  uint64_t name_hash = hash_64_combine(hash_str_32(name), eid);
  hash_key_t key = hash_combine_64(name_hash, compid);
  subject_t* s = HashGet(&SUBJECTS.map, key);

  observer_t* obs = (observer_t*)GameMalloc("SubjectAddObserver", sizeof(observer_t));
  if (!obs || !s) return; // Handle OOM in production

  strcpy(obs->name, oname);
  obs->callback = cb;
  obs->data = data;
  obs->next = s->observers;
  s->observers = obs;
}

void SubjectAddObserver(const char* name, const char* oname, ObserverCB cb, void* data){
  subject_t* s = SubjectGetEntry(name, "ADD OBSERVER");

  observer_t* obs = (observer_t*)GameMalloc("SubjectAddObserver", sizeof(observer_t));
  if (!obs || !s) return; // Handle OOM in production

  strcpy(obs->name, oname);
  obs->callback = cb;
  obs->data = data;
  obs->next = s->observers;
  s->observers = obs;
}

void SubjectRemoveObserver(subject_t* s, ObserverCB cb, void* data){
  observer_t* current = s->observers;
  observer_t* prev = NULL;

  while (current) {
    if (current->callback == cb && current->data == data) {
      if (prev) {
        prev->next = current->next;
      } else {
        s->observers = current->next;
      }
      GameFree("SubjectRemoveObserver", current);
      return;
    }
    prev = current;
    current = current->next;
  }
}

void SubjectRunNotify(subject_t* s, void* data){
  observer_t* current = s->observers;
  while (current) {
    current->callback(current->data, s, data);
    current = current->next;
  }
}

void SubjectNotify(const char* name, void* data){
  subject_t* s = SubjectGetEntry(name, "NOTIFY"); 
  if(!s)
    return;

  SubjectRunNotify(s, data);
}

void SubjectDestroy(subject_t* s){
  observer_t* current = s->observers;
  while (current) {
    observer_t* next = current->next;
    GameFree("SubjectDestroy", current);
    current = next;
  }
  s->observers = NULL;
}
