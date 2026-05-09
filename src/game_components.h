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

typedef uint64_t comp_id_t;

typedef enum ComponentType {
    COMP_NONE = 0,
    // Engine built-in components
    COMP_POSITION,
    COMP_PHYSICS,
    COMP_SPRITE,
    COMP_ANIMATION,
    COMP_CAMERA,
    COMP_INPUT,
    
    COMP_CORE_MAX,           // ← Everything below this is engine-reserved
    COMP_GAME_START
} ComponentType;
#endif
