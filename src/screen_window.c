#include "screens.h"


void InitScreenWindow(float w, float h, const char* name){
  InitWindow(w, h, name);
  window_resize_t resize = {w, h};
  SubjectStore("WINDOW", &resize);
}
