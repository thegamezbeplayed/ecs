#ifndef __GAME_CTRL__
#define __GAME_CTRL__

#include "game_utils.h"

#define MAX_MACROS  24 
#define MAX_ACTIONS 16
#define MAX_PHASE_ACTIONS 128
DEFINE_EVENT_SPACE(InputEvent, EVENT_INPUT_BASE)

typedef struct action_key_s action_key_t;
typedef struct input_s input_t;

typedef BehaviorStatus (*ActionKeyCallback)(input_t* gi, KeyboardKey k);

struct action_key_s{
  char              name[MAX_NAME_LEN];
  KeyboardKey       key;
  ActionType        type;
  Vector2           dir;
};

typedef hash_map_t macro_map_t;
void InitMacroKeys(int);
void RegisterMacro(action_key_t*);
action_key_t* InputGetAction(KeyboardKey key);

struct input_s{
  KeyboardKey     last_key;
  int             turn, frames;
  int             angle;
  ActionType      last_act;
  action_key_t    action_keys[MAX_MACROS];
};

input_t* InitInput(void);
bool InputCheck(input_t* gi, Entity e);
#endif
