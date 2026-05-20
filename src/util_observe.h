#ifndef __UTIL_OBSERVE__
#define __UTIL_OBSERVE__

#include "observer_types.h"
#include "game_common.h"
#include "components.h"

#define  MAX_SUBJECT_STORE  64
#define MAX_LISTENERS       4 
#define MAX_SUBJ_OBS        8 
// Forward declarations

typedef enum{
  OBS_NONE,
  OBS_COMP,
  OBS_DEFINE,
  OBS_ALL
}ObserveType;

typedef struct subject_s subject_t;
typedef struct observer_s observer_t;

typedef void (*ObserverCB)(void* obs_data, void* sub, void* ev_data);

struct observer_s {
  char        name[MAX_NAME_LEN];  
  ObserveType type;
  ObserverCB  callback;
  void* data;           // User data passed to callback (e.g., component pointer)
  observer_t* next;
};

typedef struct{
  char        name[MAX_NAME_LEN];
  ObserveType type;
  int         num_subj;
  comp_id_t   relation;
  char        subjects[MAX_SUBJ_OBS][MAX_NAME_LEN];
  int         num_obs;
  char        observers[MAX_LISTENERS][MAX_NAME_LEN];
}component_observer_t;

typedef struct{
  char      name[MAX_NAME_LEN];
  comp_id_t comp;
}subject_component_t;

struct subject_s {
  char        name[MAX_NAME_LEN];
  observer_t* observers;
};

void InitSubject(subject_t* subject);
void SubjectAddObserver(const char*, const char*, ObserverCB, void*) ;
void SubjectAddObserverByComponent(const char*, uint32_t, comp_id_t, const char*, ObserverCB, void*);
void SubjectRemoveObserver(subject_t*, ObserverCB, void*); // Optional
void SubjectRunNotify(subject_t* s, void* data);
void SubjectNotify(const char*, void* eventdata);
void SubjectDestroy(subject_t* subject); // Cleanup

typedef struct{
  hash_key_t  key;
  subject_t*  subject;
  void*       data;
}subject_store_t;

typedef struct{
  bool              initialized;
  hash_map_t        map, stored;
  int               cap, num_store;
}subject_pool_t;
extern subject_pool_t SUBJECTS;
subject_t* SubjectRegister(const char* name);
void SubjectStore(const char* name, void*);

void InitSubjectPool(int cap);
#endif
