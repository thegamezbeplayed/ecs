#include "util_observe.h"

subject_pool_t SUBJECTS;

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

subject_t* SubjectRegister(const char* name){
  subject_t* existing = SubjectGetEntry(name, "REGISTER");
  if(existing)
    return existing;

  hash_key_t hash = hash_str_64(name);
  subject_t* s = GameCalloc("SubjectRegister", 1, sizeof(subject_t));
  strcpy(s->name, name);
  HashPut(&SUBJECTS.map, hash, s);

  return s;
}

void SubjectAddObserver(const char* name, const char* oname, ObserverCB cb, void* data){
  subject_t* s = SubjectGetEntry(name, "ADD OBSERVER");

  observer_t* obs = (observer_t*)GameMalloc("SubjectAddObserver", sizeof(observer_t));
  if (!obs || !s) return; // Handle OOM in production

  TraceLog(LOG_INFO, "=== %s Observing %s ===", oname, name);
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
    TraceLog(LOG_INFO, "=== OBSERVER NOTIFY ===\n Subject %s Observer %s", s->name, current->name);
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
