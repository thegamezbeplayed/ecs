#ifndef __GAME_UTIL__
#define __GAME_UTIL__

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "game_common.h"
#include <stdint.h>

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

//====EVENTS===>
typedef enum{
  EVENT_TILE_COLLISION,
  EVENT_TILE_MOVE,
  EVENT_TILE_DIST,
  EVENT_TILE_END,
  EVENT_GAME_PROCESS,
  EVENT_INTERACTION,
  EVENT_PLAY_SFX,
  EVENT_SONG_END,
  EVENT_SONG_FADE_IN,
  EVENT_SONG_FADE_OUT,
  EVENT_WAIT,
  EVENT_FINISH,
  EVENT_ENT_ACTION,
  EVENT_ENT_MOVE,
  EVENT_ENT_DIE,
  EVENT_ENTER_CELL,
  EVENT_TURN_END,
  EVENT_LEVEL_END,
  EVENT_NONE,
} EventType;

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
    EventType      event;
    EventCallback  cb;
    uint64_t       uid;
    void*          user_data;
} event_sub_t;

typedef struct {
    event_sub_t* subs;
    hash_map_t   scheduled;
    int          count, cap;
} event_bus_t;

struct event_s{
  uint64_t          uid;
  EventType         type;
  int               max, calls;
  void*             data;
  uint64_t          iuid;
  TimeFrame         timing;
  int               scheduled;
};

event_bus_t* InitEventBus(int cap);
void EventBusStep(event_bus_t* bus);
event_sub_t* EventSubscribe(event_bus_t* bus, EventType event, EventCallback cb, void* u_data);
void EventEmit(event_bus_t* bus, event_t*);
void EventRemove(event_bus_t* bus, uint64_t id);
uint64_t EventSchedule(event_bus_t* bus, event_t* e);


typedef void (*CooldownCallback)(void* );


typedef struct{
  uint64_t          uid;
  EventType         type;
  int               duration;
  int               elapsed;
  bool              is_complete;
  bool              is_recycled;
  void*             on_end_params;
  void*             on_step_params;
  CooldownCallback  on_end;
  CooldownCallback  on_step;
}cooldown_t;

cooldown_t* InitCooldown(int dur, EventType, CooldownCallback on_end_callback, void* params);
void UnloadCooldown(cooldown_t* cd);

typedef struct{
  cooldown_t  cooldowns[MAX_EVENTS];
  bool        cooldown_used[MAX_EVENTS];
}timers_t;

timers_t* InitTimers();
void UnloadTimers(timers_t* ev);
int AddTimer(timers_t* pool, cooldown_t* cd);
void StepTimers(timers_t* pool);
void StartTimer(timers_t* pool, EventType type);
void ResetTimer(timers_t* pool, EventType type);
bool CheckTimer(timers_t* pool, EventType type);

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

#endif
