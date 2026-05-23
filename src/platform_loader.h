#ifndef PLATFORM_LOADER_H
#define PLATFORM_LOADER_H

#include <stdbool.h>
#include "app_resource.h"

void PlatformLoaderStart(LoadQueue* queue);
float PlatformLoaderProgress(LoadQueue* queue);
bool PlatformLoaderFinished(LoadQueue* queue);

#endif
