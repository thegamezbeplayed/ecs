#if defined(PLATFORM_DESKTOP)

#include "platform_loader.h"
#include <pthread.h>
#include <stdatomic.h>
#include "scene_loader.h"

static void* DesktopBackgroundLoader(void* arg)
{
  LoadQueue* queue = (LoadQueue*)arg;

  for (int i = 0; i < queue->count; i++) {
    LoadJob* job = &queue->jobs[i];

    job->image = LoadImage(job->path);
    atomic_store(&job->ready, true);
  }

  SceneLoadResources();
  atomic_store(&queue->finished, true);
  return NULL;
}

void PlatformLoaderStart(LoadQueue* queue)
{
  pthread_mutex_init(&queue->mutex, NULL);
  atomic_store(&queue->load_count, 0);
  atomic_store(&queue->finished, false);

  for (int i = 0; i < queue->count; i++) {
    atomic_store(&queue->jobs[i].ready, false);
    atomic_store(&queue->jobs[i].done, false);
  }

  pthread_t thread;
  pthread_create(&thread, NULL, DesktopBackgroundLoader, queue);
  pthread_detach(thread);
}

float PlatformLoaderProgress(LoadQueue* queue)
{
  for (int i = 0; i < queue->count; i++) {
    LoadJob* job = &queue->jobs[i];

    if (atomic_load(&job->ready) && !atomic_load(&job->done) && IsImageValid(job->image)) {
      job->texture = LoadTextureFromImage(job->image);
      UnloadImage(job->image);

      if (job->dest != NULL)
        *job->dest = job->texture;

      atomic_store(&job->done, true);
      atomic_fetch_add(&queue->load_count, 1);
    }
  }

  if (queue->count == 0)
    return 1.0f;

  return (float)atomic_load(&queue->load_count) / queue->count;
}

bool PlatformLoaderFinished(LoadQueue* queue)
{
  return atomic_load(&queue->finished) && atomic_load(&queue->load_count) >= queue->count;
}

#endif
