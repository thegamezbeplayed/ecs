#ifndef __APP_RES__
#define __APP_RES__
#include "raylib.h"
#include <pthread.h>
#include <stdatomic.h>
#include "asset_define.h"

// One resource item
typedef struct {
    char            path[64];
    Image           image;           // Loaded on background thread
    Texture2D       texture;     // Uploaded on main thread
    Texture2D*      dest;     // Uploaded on main thread
    atomic_bool     ready;     // Signal that CPU image is ready
    atomic_bool     done;      // Signal that GPU texture is uploaded
} LoadJob;

// Simple queue (for multiple resources)
#define MAX_JOBS 32
typedef struct {
    pthread_mutex_t mutex;
    LoadJob         jobs[MAX_JOBS];
    int             count;
    atomic_bool     finished;
    atomic_int      load_count;
} LoadQueue;
float GetLoadingProgress(void);

extern LoadQueue loader;

#endif
