#ifndef __UI_DEF__
#define __UI_DEF__
#include "raylib.h"
#include <stddef.h>
#include "ui_enum.h"
#include "game_define.h"

typedef struct{
  UILayout    type;
  UIAlignment align;
  Vector2     offset;
}layout_t;
bool LayoutInit(void* comp, component_entry_t* j);

typedef struct{
  UIAlignment algin;
  float       size, spacing;
  Color       col;
  size_t      len;
  char*       str;
}text_t;
bool TextInit(void* comp, component_entry_t* j);

typedef struct{
  ElementState  state, prev;
}element_state;
bool ElementInitState(void* comp, component_entry_t* j);
bool ElementSetType(void* comp, component_entry_t* j);

typedef struct{
  UITexture   type;
}texture_t;
bool TextureInit(void* comp, component_entry_t* j);

typedef struct{
  ElementState  state;
  UIEventID     event;
}interact_t;
#endif
