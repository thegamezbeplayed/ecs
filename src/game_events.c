#include "process_define.h"

payload_t* InitPayload(int count, param_t params[count]){
  payload_t* p = GameCalloc("InitPayload", 1, sizeof(payload_t));

  p->params =  GameCalloc("InitPayload", 2, sizeof(param_t));

  for(int i = 0; i < count; i++){
    param_t* e = &p->params[p->count++];

    *e = params[i];
  }

  return p;
}

payload_t* InitPayloadSingle(param_t param){
  payload_t* p = GameCalloc("InitPayloadSingle", 1, sizeof(payload_t));

  p->params = GameCalloc("InitPayload", 1, sizeof(payload_t));

  param_t* e = &p->params[p->count++];
  *e = param;

  return p;
}

cooldown_t* InitCooldown(int dur, notification type){
  cooldown_t* cd = GameCalloc("InitCooldown", 1, sizeof(cooldown_t)); 

  *cd = (cooldown_t){
    .type = type,
      .is_complete = false,
      .duration = dur,
      .elapsed = 0,
  };
  
  return cd;
}
void UnloadCooldown(cooldown_t* cd){
  if(!cd)
    return;

  *cd = (cooldown_t){0};
}

void UnloadTimers(timers_t* ev){
  for (int i = 0; i < MAX_EVENTS; i++){
    if(!ev->cooldown_used[i])
      continue;
    UnloadCooldown(&ev->cooldowns[i]);
    ev->cooldown_used[i] = false;
  }
}

int AddTimer(timers_t* pool, cooldown_t* cd){
  if(CheckTimer(pool,cd->type))
    return -1;
  
  for (int i = 0; i < MAX_EVENTS; i++){
    if(pool->cooldown_used[i]){
      if(pool->cooldowns[i].type <= 0){
        pool->cooldowns[i] = *cd;
        pool->cooldown_used[i] = true;
        return i;
      }
      else
        continue;
    }
    else{
      pool->cooldowns[i] = *cd;
      pool->cooldown_used[i] = true;
      return i;
    }
  }

  return -1;
}

timers_t* InitTimers(){
  timers_t* ev = GameMalloc("InitTimers", sizeof(timers_t));
  *ev =  (timers_t) { 0 };

  for(int i = 0; i < MAX_EVENTS; i++){
    ev->cooldown_used[i] = false;
    ev->cooldowns[i].type = -1;
  }

  return ev;
}

int GetEventIndex(timers_t* pool, notification type){
  for(int i = 0; i<MAX_EVENTS; i++){
    if(!pool->cooldown_used[i])
      continue;

    if(pool->cooldowns[i].type != type)
      continue;


    return i;
  }
  
  return -1;
}


bool CheckTimer(timers_t* pool, notification type){
  for(int i = 0; i<MAX_EVENTS; i++){
    if(!pool->cooldown_used[i])
      continue;

    if(pool->cooldowns[i].type != type)
      continue;

    if(pool->cooldowns[i].is_complete)
      return false;

    return true;
  }

  return false;
}

void ResetTimer(timers_t* pool, notification type){
  for (int i = 0; i < MAX_EVENTS; i++){
    if(!pool->cooldown_used[i])
      continue;

    if(pool->cooldowns[i].type != type)
      continue;

    pool->cooldowns[i].is_complete = false;
    pool->cooldowns[i].elapsed = 0;
  }

}

void StartEvent(timers_t* pool, notification type){
  for (int i = 0; i < MAX_EVENTS; i++){
    if(!pool->cooldown_used[i])
      continue;
 
    if(pool->cooldowns[i].type != type)
      continue;
  }
}

void StepTimers(timers_t* pool){
  for (int i = 0; i < MAX_EVENTS; i++){
    if(!pool->cooldown_used[i])
      continue;

    if(pool->cooldowns[i].type <= 0)
      continue;

    if(pool->cooldowns[i].is_complete && !pool->cooldowns[i].is_recycled){
      pool->cooldowns[i]=(cooldown_t){0};
      pool->cooldowns[i].type = -1;
      pool->cooldown_used[i] = false;
      continue;
    }
   /* 
    if(pool->cooldowns[i].is_complete){
      if(!pool->cooldowns[i].is_recycled)
        continue;

      pool->cooldowns[i].is_complete = false;
      pool->cooldowns[i].elapsed = 0;

      continue;
    }
    */
    if(pool->cooldowns[i].elapsed >= pool->cooldowns[i].duration){
      pool->cooldowns[i].is_complete = true;
      pool->cooldowns[i].elapsed = 0;
      if(pool->cooldowns[i].on_end){
        pool->cooldowns[i].on_end(pool->cooldowns[i].on_end_params);
      }
      continue;
    }

    if(pool->cooldowns[i].is_complete && pool->cooldowns[i].is_recycled)
      continue;

    pool->cooldowns[i].elapsed++;
    if(pool->cooldowns[i].on_step)
      pool->cooldowns[i].on_step(pool->cooldowns[i].on_step_params);
  }
}

enum { EVENT_POOL_END = UINT32_MAX };

event_bus_t* InitEventBusEx(int sub_cap, int event_cap){
  if (sub_cap <= 0) sub_cap = 1;
  if (event_cap <= 0) event_cap = 1;

  event_bus_t* bus = GameCalloc("InitEventBus",1,sizeof(event_bus_t));
  *bus = (event_bus_t){
    .cap = sub_cap,
    .event_cap = event_cap,
    .free_head = 0,
    .subs = GameCalloc("InitEventBus", sub_cap, sizeof(event_sub_t)),
    .events = GameCalloc("InitEventBus", event_cap, sizeof(event_slot_t)),
    .due_events = GameCalloc("InitEventBus", event_cap, sizeof(uint64_t))
  };

  for (int i = 0; i < event_cap; i++) {
    bus->events[i].next_free = (i == event_cap - 1) ? EVENT_POOL_END : (uint32_t)(i + 1);
    bus->events[i].generation = 1;
    bus->events[i].event.pool_index = (uint32_t)i;
  }

  HashInit(&bus->scheduled, next_pow2_int(event_cap*2));
  return bus;
}

event_bus_t* InitEventBus(int cap){
  return InitEventBusEx(cap, MAX_EVENTS);
}

void EventBusUnload(event_bus_t* bus){
  if (!bus)
    return;

  for (int i = 0; i < bus->event_cap; i++){
    if (bus->events[i].in_use)
      EventRelease(bus, &bus->events[i].event);
  }

  HashFree(&bus->scheduled);
  GameFree("EventBusUnload", bus->due_events);
  GameFree("EventBusUnload", bus->events);
  GameFree("EventBusUnload", bus->subs);
  GameFree("EventBusUnload", bus);
}

void EventBusEnsureCap(event_bus_t* bus){
  if (bus->count < bus->cap)
    return;

  TraceLog(LOG_WARNING, "=== EVENT BUS ===\n subscription cap reached (%d)", bus->cap);
}

event_t* EventAcquire(event_bus_t* bus){
  if (!bus || bus->free_head == EVENT_POOL_END) {
    if (bus) bus->dropped_count++;
    TraceLog(LOG_WARNING, "=== EVENT BUS ===\n event pool exhausted");
    return NULL;
  }

  uint32_t index = bus->free_head;
  event_slot_t* slot = &bus->events[index];

  bus->free_head = slot->next_free;
  slot->next_free = EVENT_POOL_END;
  slot->in_use = true;
  bus->event_count++;
  if (bus->event_count > bus->active_peak)
    bus->active_peak = bus->event_count;

  memset(&slot->event, 0, sizeof(event_t));
  slot->event.pool_index = index;
  slot->event.generation = slot->generation;
  slot->event.max = -1;
  return &slot->event;
}

void EventRelease(event_bus_t* bus, event_t* e){
  if (!bus || !e || e->pool_index >= (uint32_t)bus->event_cap)
    return;

  event_slot_t* slot = &bus->events[e->pool_index];
  if (!slot->in_use || slot->generation != e->generation)
    return;

  uint32_t index = e->pool_index;

  if (e->owns_data && e->data)
    GameFree("EventRelease", e->data);

  slot->in_use = false;
  slot->generation++;
  memset(&slot->event, 0, sizeof(event_t));
  slot->event.pool_index = index;
  slot->next_free = bus->free_head;
  bus->free_head = index;
  if (bus->event_count > 0)
    bus->event_count--;
}

event_t* InitEvent(event_bus_t* bus, uint64_t event, void* data, int uid){
  event_t* ev = EventAcquire(bus);
  if (!ev)
    return NULL;

  uint32_t pool_index = ev->pool_index;
  uint32_t generation = ev->generation;
  uint64_t guid = hash_combine_64(event, hash_64_from_int(uid));
  *ev = (event_t){
    guid, event, -1, 0, data, uid, TF_NONE, 0,
    pool_index, generation, false, 0
  };
  return ev;
}

event_t* InitEventCopy(event_bus_t* bus, uint64_t event, const void* data, size_t data_size, int uid){
  event_t* ev = InitEvent(bus, event, NULL, uid);
  if (!ev)
    return NULL;

  if (data && data_size > 0) {
    ev->data = GameCalloc("InitEventCopy", 1, data_size);
    if (!ev->data) {
      EventRelease(bus, ev);
      return NULL;
    }
    memcpy(ev->data, data, data_size);
    ev->owns_data = true;
    ev->data_size = data_size;
  }

  return ev;
}

void EventBusStep(event_bus_t* bus){
  if (!bus)
    return;

  hash_iter_t iter;
  HashStart(&bus->scheduled, &iter);

  hash_slot_t* s;
  int due_count = 0;

  while((s = HashNext(&iter))){
    event_t* e = s->value;
    if (!e)
      continue;

    switch(e->timing){
      case TF_TURN:
        //if(e->scheduled > WorldGetTurn())
          continue;
        break;
      case TF_UPDATE:
        if(e->scheduled > WorldGetTime())
          continue;
        break;
      default:
        break;

    }

    if (due_count < bus->event_cap)
      bus->due_events[due_count++] = e->uid;
  }

  for (int i = 0; i < due_count; i++) {
    uint64_t uid = bus->due_events[i];
    event_t* e = HashGet(&bus->scheduled, uid);
    if (!e)
      continue;

    EventEmit(bus, e);
    HashRemove(&bus->scheduled, uid);
    EventRelease(bus, e);
  }
}

event_sub_t* EventSubscribe(event_bus_t* bus, notification event, EventCallback cb, void* u_data){
  if (!bus || !cb)
    return NULL;

  EventBusEnsureCap(bus);
  if (bus->count >= bus->cap)
    return NULL;

  event_sub_t* sub =  &bus->subs[bus->count++];
  *sub = (event_sub_t){
    .event = event,
    .cb = cb,
    .eid = -1,
    .user_data = u_data
  };

  return sub;
}

void EventRemove(event_bus_t* bus, uint64_t id){
  EventUnsubscribeByOwner(bus, id);
}

void EventUnsubscribeByOwner(event_bus_t* bus, uint64_t id){
  if (!bus) return;

  for (int i = 0; i < bus->count;) {
    if (bus->subs[i].eid != id) {
      i++;
      continue;
    }

    bus->subs[i] = bus->subs[bus->count - 1];
    bus->count--;
  }
}

void EventUnsubscribe(event_bus_t* bus, event_sub_t* sub)
{
  if (!bus || !sub) return;

  // Find the subscription in the array
  int index = -1;
  for (int i = 0; i < bus->count; i++) {
    if (&bus->subs[i] == sub) {
      index = i;
      break;
    }
  }

  if (index < 0) return; // not found

  // Move last element into the removed slot
  bus->subs[index] = bus->subs[bus->count - 1];
  bus->count--;
}

void EventEmit(event_bus_t* bus, event_t* e){
  if (!bus || !e)
    return;

  for (int i = 0; i < bus->count; i++) {
    if (bus->subs[i].event != e->type)
      continue;

    if(e->max != -1 && e->calls >= e->max)
      break;

    if (bus->subs[i].eid != -1
       && bus->subs[i].eid != e->eid)
      continue;

    bus->subs[i].cb(e, bus->subs[i].user_data);
    e->calls++;
  }
}

uint64_t EventSchedule(event_bus_t* bus, event_t* e){
  if (!bus || !e)
    return 0;

  uint64_t uid = hash_combine_64(hash_str_64("EVENT"),
      hash_combine_64(e->uid, WorldGetTime()));

  e->uid = uid;

  HashPut(&bus->scheduled, uid, e);
  return uid;
}

bool EventUnschedule(event_bus_t* bus, uint64_t uid){
  if (!bus || uid == 0)
    return false;

  event_t* e = HashGet(&bus->scheduled, uid);
  if (!e)
    return false;

  HashRemove(&bus->scheduled, uid);
  EventRelease(bus, e);
  return true;
}

notification_t* NotificationGet(notification_pool_t* p, hash_key_t key ){
  if(p->count == 0)
   return 0;

  return HashGet(&p->map, key);
}

notification NotificationCheck(notification_pool_t* p, hash_key_t key ){
  if(p->count == 0)
   return 0; 
  
  return HashKey(&p->map, key);
}

notification_pool_t* InitNotifications(int cap){
  notification_pool_t* n = GameCalloc("InitNotifications", 1, sizeof(notification_pool_t));

  n->cap = cap;
  HashInit(&n->map, next_pow2_int(cap*2));

  return n;
}

notification_t* RegisterNotification(notification_pool_t* p, char* name){
  notification n = hash_str_64(name);
  hash_key_t key = NotificationCheck(p, n);
  notification_t* notif = NULL;
  
  if(key > 0)
    return NotificationGet(p, key);
  
  notif = GameCalloc("RegisterNotification", 1, sizeof(notification_t));
  notif->hash = n;
  strcpy(notif->name, name);
  HashPut(&p->map, n, notif);
  p->count++;
  return notif; 
}

hash_key_t MakeInteractionKey(notification n, uint32_t source, uint32_t target){
  return hash_event(source, target, n);

}

void InteractionRegister(interaction_pool_t* p, uint32_t source, uint32_t target, notification n,  float duration){
  uint64_t key = MakeInteractionKey(n, source, target);
  uint32_t hash = (uint32_t)(key ^ (key >> 32) ^ (key >> 16));

  int current_time = WorldGetTime();
  for (int i = 0; i < 12; ++i)
  {
    uint32_t idx = (hash + i) & (MAX_INTERACTIONS - 1);
    interaction_t* slot = &p->entries[idx];

    if (slot->key == 0 || slot->key == key)
    {
      slot->key = key;
      slot->type = n;
      slot->expiration = current_time + duration;
      return;
    }
  }
  // Table full → ignore (very rare with 4096 slots)
}

bool InteractionCheck(interaction_pool_t* p, uint32_t source, uint32_t target, notification n)
{
  uint64_t key = MakeInteractionKey(n, source, target);
  uint32_t hash = (uint32_t)(key ^ (key >> 32) ^ (key >> 16));  // good mixing
  int current_time = WorldGetTime();

  for (int i = 0; i < 12; ++i)   // limited linear probing
  {
    uint32_t idx = (hash + i) & (MAX_INTERACTIONS - 1);
    interaction_t* slot = &p->entries[idx];

    if (slot->key == 0)                    // empty slot
      return true;

    if (slot->key == key){
      int remaining = slot->expiration - current_time;
      if(remaining > 0)
        return false;

      slot->key = 0;
      return true;

    }
  }

  return true;   // table full → allow interaction (rare)
}

void InteractionStep(interaction_pool_t* p){
  p->current_frame = WorldGetTime();
}
