#ifndef __ASS_RES__
#define __ASS_RES__
#include <string.h>
#include <stdlib.h>
#include "game_resources.h"

char* GetFileStem(const char* filename);

typedef enum{
  RES_NONE,
  RES_JSON,
  RES_SPRITE,
  RES_ANIM,
  RES_SFX,
  RES_MUSIC,
  RES_DONE
}ResourceCategory;

typedef enum{
  RES_BLANK,
  RES_ASEPRITE,
  RES_ALL
}ResourceType;

typedef struct ResourceRef {
  ResourceCategory  cat;
  ResourceType      type, subtype;
  char*       path;      // "assets/tilesets/forest.png"
  char*       name;      // optional identifier "forest_tiles"
  SheetID           sheet;     // for multi-tilesheet scenes
  void*             dest;
} ResourceRef;

typedef struct{
  int         count, cap;
  ResourceRef *refs;
}ResourcePool;

void ResourceInit(int count);
void ResourceLoad(ResourceRef ref);


#endif
