#ifndef __UTIL_OBSERVE__
#define __UTIL_OBSERVE__

#include "observer_types.h"

#define MAX_LISTENERS 32
// Forward declarations
typedef struct subject_s subject_t;
typedef struct observer_s observer_t;

typedef void (*ObserverCB)(void* obs_data, void* sub, void* ev_data);

struct observer_s {
    ObserverCB callback;
    void* data;           // User data passed to callback (e.g., component pointer)
    observer_t* next;
};

// Subject (the thing being observed)
struct subject_s {
    observer_t* observers;
    // Optional: you can add a type/ID for filtering if needed
};

void SubjectInit(subject_t* subject);
void SubjectAddObserver(subject_t*, ObserverCB, void*) ;
void SubjectRemoveObserver(subject_t*, ObserverCB, void*); // Optional
void SubjectNotify(subject_t* subject, void* eventdata);
void SubjectDestroy(subject_t* subject); // Cleanup
#endif
