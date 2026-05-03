#ifndef __GAME_COMP__
#define __GAME_COMP__

#include "screens.h"
#include "game_physics.h"
#include "game_behaviors.h"
#include "game_assets.h"
#include "game_control.h"

#define MAX 1024
#define MAX_COMPONENTS 512
#define MAX_PLAYERS 2

#define COMPONENT_CAP 16
#define COMP_MASK 0xFFFFFFFF

typedef uint64_t comp_id_t;
typedef void (*ComponentImportFn)(void*, const char*);
void ComponentMap(const char* name, comp_id_t* id, ComponentImportFn);

ComponentImportFn ComponentMapFn(const char* name);

comp_id_t* ComponentMapGetID(const char* name);
void InitComponentMap(int size);

#endif
