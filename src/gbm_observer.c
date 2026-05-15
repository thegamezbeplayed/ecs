#include "util_observe.h"
#include "game_tools.h"

void SubjectInit(subject_t* subject){
  subject->observers = NULL;
}

void SubjectAddObserver(subject_t* s, ObserverCB cb, void* data){
  observer_t* obs = (observer_t*)GameMalloc("SubjectAddObserver", sizeof(observer_t));
  if (!obs) return; // Handle OOM in production

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

void SubjectNotify(subject_t* s, void* data){
  observer_t* current = s->observers;
  while (current) {
    current->callback(current->data, s, data);
    current = current->next;
  }
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
