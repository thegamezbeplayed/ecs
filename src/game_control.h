#ifndef __GAME_CTRL__
#define __GAME_CTRL__

#include "game_utils.h"

#define MAX_ACTIONS 16
#define MAX_PHASE_ACTIONS 128
DEFINE_EVENT_SPACE(InputEvent, EVENT_INPUT_BASE)

typedef struct action_key_s action_key_t;

typedef struct action_s action_t;
typedef struct input_s input_t;

typedef void (*ActionCb)(void*);
typedef ActionStatus (*ActionFn)(action_t* a);
struct action_s{
  uint64_t        id;
  ActionType      type;
  ActionStatus    status;
  TurnPhase       phase;
  int             num_params;
  param_t         params[10];
  int             turn, weight, score;
  int             initiative;
  ActionFn        fn;
  ActionCb        cb;
};

typedef struct{
  ActionStatus    status;
  int             count, cap, charges, allowed;
  action_t*       entries;
}action_queue_t;

typedef struct{
  int             count, cap;
  ActionStatus    status;
  TurnPhase       phase;
  action_t*       entries[MAX_PHASE_ACTIONS];
}action_round_t;

typedef struct{
  int             turn;
  TurnPhase       phase, next;
  action_round_t  round[TURN_ALL];
}turn_action_manager_t;
extern turn_action_manager_t ActionMan;

BehaviorStatus ActionExecute(ActionType, action_t*);
void InitActionManager(void);

ActionStatus QueueAction(action_queue_t*, action_t*);

void ActionFree(action_t* a);

void ActionManagerSync(void);
bool ActionInput(void);

typedef BehaviorStatus (*ActionKeyCallback)(input_t* gi, KeyboardKey k);

struct action_key_s{
  ActionType        action;
  int               num_keys;
  KeyboardKey       keys[12];
  ActionKeyCallback fn;
  int               binding;
};

struct input_s{
  int             turn, frames;
  int             angle;
  Cell            step;
  action_key_t    actions[ACT_DONE];
};
input_t* InitInput(void);

bool InputCheck(input_t*, int turn);



#endif
