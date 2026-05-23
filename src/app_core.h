#ifndef APP_CORE_H
#define APP_CORE_H

#include <stdbool.h>

typedef struct {
  int width;
  int height;
  int fps;
  const char* title;
} AppConfig;

void AppInit(AppConfig config);
void AppFrame(bool wait);
void AppShutdown(void);
void AppOnResize(int width, int height);
bool AppWantsQuit(void);
bool AppShouldThrottleFrame(void);
float AppGetLoadingProgress(void);
bool AppIsLoaded(void);

#endif
