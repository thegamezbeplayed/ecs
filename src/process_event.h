#ifndef __PROC_EVENT__
#define __PROC_EVENT__

#include "game_common.h"
#include "game_utils.h"
//===WORLD_T===>

typedef struct{
 notification_pool_t* notifications;
  interaction_pool_t   interactions;
  event_bus_t          *bus;
}game_manage_t;
extern game_manage_t GM;

void SubscribeEntity(uint64_t, EventCallback, void*, int);
void TargetSubscribe(uint64_t, EventCallback cb, void* data, int);
void Subscribe(uint64_t, EventCallback cb, void* data);
void ScheduleEvent(uint64_t, void* data, uint64_t uid, TimeFrame, int);
void GameEvent(uint64_t, void*, uint64_t);
void GameUnloadEvents();
static bool GameCheckInteraction(uint32_t a, uint32_t b, notification n){
  return InteractionCheck(&GM.interactions, a, b, n);
}

static void GameInteraction(uint32_t a, uint32_t b, notification n,  int dur){
  InteractionRegister(&GM.interactions, a, b, n, dur);
}
#endif
