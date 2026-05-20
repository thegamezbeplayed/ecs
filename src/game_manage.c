#include "process_event.h"
#include "scene_loader.h"

world_t world;
game_manage_t GM;

event_bus_t* GameBus(void){
  return GM.bus;
}

void InitGameEvents(){
  GM.bus = InitEventBusEx(MAX_EVENT_SUBS, MAX_EVENTS);
  GM.notifications = InitNotifications(MAX_INTERACTIONS);
}     

void GameUnloadEvents(){
  EventBusUnload(GM.bus);
  GM.bus = NULL;
}

void GameOnStateChange(GameState state){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STATE), &world , state);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SET), &world , state);
}

void Subscribe(uint64_t event, EventCallback cb, void* data){
  event_sub_t* sub = EventSubscribe(GameBus(), event , cb, data);
  if (!sub) return;
  sub->eid = -1;
}

void SubscribeEntity(uint64_t event, EventCallback cb, void* data, int id){
  event_sub_t* sub = EventSubscribe(GameBus(), event, cb, data);
  if (!sub) return;
  sub->eid = id;
} 

void TargetSubscribe(uint64_t event, EventCallback cb, void* data, int id){
  event_sub_t* sub = EventSubscribe(GameBus(), event, cb, data);
  if (!sub) return;
  sub->eid = id;
} 

void ScheduleEvent(uint64_t event, void* data, uint64_t uid, TimeFrame tf, int step){
  switch(tf){
    case TF_TURN:
//      step += WorldGetTurn();
      break;
    case TF_UPDATE:
      step += WorldGetTime();
      break;
    default:
      return;
      break;
  }

  event_t* ev = InitEvent(GameBus(), event, data, uid);
  if (!ev) return;

  ev->timing = tf;
  ev->scheduled = step;

  EventSchedule(GameBus(), ev);
}

void GameEvent(uint64_t event, void* data, uint64_t uid){
  event_bus_t* bus = GameBus();
  if(!bus || bus->count == 0)
    return;

  event_t* ev = InitEvent(bus, event, data, uid);
  if (!ev) return;
  EventEmit(bus, ev);
  EventRelease(bus, ev);
}

