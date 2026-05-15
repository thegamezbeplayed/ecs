#include "screens.h"

subject_t window;

void InitScreenWindow(float w, float h, const char* name){
  SubjectInit(&window);
  InitWindow(w, h, name);
  window_resize_t resize = {w, h};
  SubjectNotify(&window, &resize);
}
