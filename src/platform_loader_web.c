#if defined(PLATFORM_WEB)

#include "platform_loader.h"
#include <stdatomic.h>
#include "scene_loader.h"

void PlatformLoaderStart(LoadQueue* queue)
{
  atomic_store(&queue->load_count, 0);
  atomic_store(&queue->finished, false);

  for (int i = 0; i < queue->count; i++) {
    LoadJob* job = &queue->jobs[i];

    if (job->dest != NULL)
      *job->dest = LoadTexture(job->path);

    atomic_store(&job->ready, true);
    atomic_store(&job->done, true);
    atomic_fetch_add(&queue->load_count, 1);
  }

  SceneLoadResources();
  atomic_store(&queue->finished, true);
}

float PlatformLoaderProgress(LoadQueue* queue)
{
  if (queue->count == 0)
    return 1.0f;

  return (float)atomic_load(&queue->load_count) / queue->count;
}

bool PlatformLoaderFinished(LoadQueue* queue)
{
  return atomic_load(&queue->finished) && atomic_load(&queue->load_count) >= queue->count;
}

#endif
