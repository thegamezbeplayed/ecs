#ifndef __GAME_UTIL__
#define __GAME_UTIL__

#include "game_types.h"
#include "game_common.h"
#include "game_define.h"

#define MAX_BEHAVIOR_TREE 12
#define MAX_EVENTS 512

#define COMBO_KEY(a, b) ((a << 8) | b)
#define CALL_FUNC(type, ptr, ...) ((type)(ptr))(__VA_ARGS__)
#define MAKE_ADAPTER(name, T) \
    static void name##_Adapter(void *p) { name((T)p); }
#pragma once
#ifdef __EMSCRIPTEN__
  #include <emscripten/emscripten.h>
#else
  #define EMSCRIPTEN_KEEPALIVE
#endif
void UploadScore(void);

//====FILE & STRINGS====>
char* GetFileStem(const char* filename);
//<==========
//
static inline void DO_NOTHING(void){}
static inline bool BOOL_DO_NOTHING(){return false;}
static inline const char* CHAR_DO_NOTHING(){return "\0";}

typedef uint64_t notification;
    
typedef struct{
  char          name[MAX_NAME_LEN];
  notification  hash;
}notification_t;

typedef struct{
  int         count, cap;
  hash_map_t  map;
}notification_pool_t;
notification_pool_t* InitNotifications(int cap);
notification_t* RegisterNotification(notification_pool_t*, char*);
//====EVENTS===>

typedef enum{
  TF_NONE,
  TF_TURN,
  TF_UPDATE,
}TimeFrame;

typedef struct event_s event_t;
typedef void (*EventCallback)(
    event_t    *event,
    void*      user_data
);

typedef struct {
    notification   event;
    EventCallback  cb;
    int            eid;
    void*          user_data;
} event_sub_t;

typedef struct {
    event_sub_t* subs;
    hash_map_t   scheduled;
    int          count, cap;
} event_bus_t;

struct event_s{
  uint64_t          uid;
  notification      type;
  int               max, calls;
  void*             data;
  int               eid;
  TimeFrame         timing;
  int               scheduled;
};
event_t* InitEvent(notification_pool_t*, char*, void*, int);
event_bus_t* InitEventBus(int cap);
void EventBusStep(event_bus_t* bus);
event_sub_t* EventSubscribe(event_bus_t* bus, notification event, EventCallback cb, void* u_data);
void EventEmit(event_bus_t* bus, event_t*);
void EventRemove(event_bus_t* bus, uint64_t id);
uint64_t EventSchedule(event_bus_t* bus, event_t* e);

typedef void (*CooldownCallback)(void* );

typedef struct{
  uint64_t          uid;
  notification      type;
  int               duration;
  int               elapsed;
  bool              is_complete;
  bool              is_recycled;
  void*             on_end_params;
  void*             on_step_params;
  CooldownCallback  on_end;
  CooldownCallback  on_step;
}cooldown_t;

cooldown_t* InitCooldown(int dur, notification);
void UnloadCooldown(cooldown_t* cd);

typedef struct{
  cooldown_t  cooldowns[MAX_EVENTS];
  bool        cooldown_used[MAX_EVENTS];
}timers_t;

timers_t* InitTimers();
void UnloadTimers(timers_t* ev);
int AddTimer(timers_t* pool, cooldown_t* cd);
void StepTimers(timers_t* pool);
void StartTimer(timers_t* pool, notification type);
void ResetTimer(timers_t* pool, notification type);
bool CheckTimer(timers_t* pool, notification type);

//INTERACTIONS_T===>
typedef struct {
  notification    type;
  uint32_t        source, target;
  cooldown_t*     timer;
} interaction_t;

typedef hash_map_t interactions;

void InitInteractions(hash_map_t* m, int cap);
hash_key_t RegisterInteraction(interactions*, interaction_t*);       
interaction_t* InitInteraction(uint32_t, uint32_t, char*, int);
bool InteractionCheck(interactions* p, uint32_t a, uint32_t b, char* type);
void InteractionStep(interactions*);
//<===BEHAVIOR TREES

//forward declare

typedef struct choice_s choice_t;
typedef struct choice_pool_s choice_pool_t;
typedef void (*OnChosen)(choice_pool_t* pool, choice_t* self);
  
void DiscardChoice(choice_pool_t* pool, choice_t* self);

typedef choice_t* (*ChoiceFn)(choice_pool_t *pool);
choice_t* ChooseByWeight(choice_pool_t* pool);
void ChoiceReduceScore(choice_pool_t* pool, choice_t* self);
  
struct choice_s{
  unsigned int id, cat;
  int          score, orig_score, cost;
  void*        context;
  uint64_t     flags;
  OnChosen     cb;
};
  
choice_t* ChoiceById(choice_pool_t* pool, int id);
  
struct choice_pool_s{
  unsigned int  id;
  int           cap, desired, count,budget, total;
  uint64_t      flags;
  ChoiceFn      choose;
  choice_t      *choices;
};

static void ShuffleChoices(choice_pool_t* pool){
  for (int i = pool->count - 1; i > 0; i--) {
    int j = rand() % (i + 1);

    choice_t tmp = pool->choices[i];
    pool->choices[i] = pool->choices[j];
    pool->choices[j] = tmp;
  }
}

choice_pool_t* StartChoice(choice_pool_t** pool, int size, ChoiceFn fn, bool* result);
void EndChoice(choice_pool_t* pool, bool reset);
choice_pool_t* InitChoicePool(int size, ChoiceFn fn);
bool AddChoice(choice_pool_t *pool, int id, int score, void *ctx, OnChosen fn);  

typedef bool (*StateComparator)(int a, int b);

typedef struct{
  int             state;
  StateComparator can;
  int             required;
}state_change_requirement_t;

static state_change_requirement_t CAN_CHANGE[STATE_END+1] = {
  {STATE_NONE, NEVER, STATE_END},
  {STATE_SPAWN, LESS_THAN, STATE_SPAWN},
  {STATE_IDLE, LESS_THAN, STATE_DIE},
  {STATE_DIE, LESS_THAN, STATE_DIE},
  {STATE_END, EQUAL_TO, STATE_DIE},
};

#endif
