#include "game_process.h"

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

event_t* InitEvent(notification_pool_t* p, uint64_t event, void* data, int uid){
  event_t* ev = GameCalloc("InitEvent",1,sizeof(event_t));

  uint64_t guid = hash_combine_64(event, hash_64_from_int(uid));
  *ev = (event_t){
    guid, event, -1, 0, data, uid
  };
  return ev;
}

event_bus_t* InitEventBus(int cap){
  event_bus_t* bus = GameCalloc("InitEventBus",1,sizeof(event_bus_t));
  *bus = (event_bus_t){
    .cap = cap,
      .subs = GameCalloc("InitEventBus", cap, sizeof(event_sub_t))
  };

  HashInit(&bus->scheduled, next_pow2_int(cap*2));
  return bus;
}

void EventBusEnsureCap(event_bus_t* bus){
  if (bus->count < bus->cap)
    return;

  int new_cap = bus->cap + 64;
  bus->subs = GameRealloc("EventBusEnsureCap", bus->subs, new_cap * sizeof(event_sub_t));
  bus->cap = new_cap;
}

void EventBusStep(event_bus_t* bus){
/*
  hash_iter_t iter;
  HashStart(&bus->scheduled, &iter);

  hash_slot_t* s;

  while((s = HashNext(&iter))){
    event_t* e = s->value;

    switch(e->timing){
      case TF_TURN:
        if(e->scheduled > WorldGetTurn())
          continue;
        break;
      case TF_UPDATE:
        if(e->scheduled > WorldGetTime())
          continue;
        break;
      default:
        break;

    }

    EventEmit(bus, e);
    HashRemove(&bus->scheduled, e->gouid);
  }
  */
}

event_sub_t* EventSubscribe(event_bus_t* bus, notification event, EventCallback cb, void* u_data){
  EventBusEnsureCap(bus);

  event_sub_t* sub =  &bus->subs[bus->count++];
  *sub = (event_sub_t){
    .event = event,
      .cb = cb,
      .user_data = u_data
  };

  return sub;
}

void EventRemove(event_bus_t* bus, uint64_t id){
  if (!bus) return;

  int index = -1;
  for (int i = 0; i < bus->count; i++) {
    if (bus->subs[i].eid != id)
      continue;
    index = i;
    break;
  }

  if (index < 0) return; // not found

  bus->subs[index] = bus->subs[bus->count - 1];
  bus->count--;

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

  if(e->max != -1 && e->calls >= e->max)
    EventRemove(bus, e->uid);
}

uint64_t EventSchedule(event_bus_t* bus, event_t* e){
  /*
  uint64_t uid = hash_combine_64(hash_string_64("EVENT"),
      hash_combine_64(e->iuid, WorldGetTime()));

  e->uid = uid;

  HashPut(&bus->scheduled, gouid, e);
  return gouid;
  */
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

  return notif; 
}

hash_key_t RegisterInteraction(interactions* p, interaction_t*entry){
  hash_key_t key = hash_event(entry->source, entry->target, entry->type);

  HashPut(p, key, entry);

  return key;
} 

interaction_t* InteractionsGetEntry(hash_map_t* m, hash_key_t key){
  return HashGet(m, key);
}

void InitInteractions(hash_map_t* m, int cap){

  HashInit(m, cap);
}

interaction_t* InitInteraction(uint32_t a, uint32_t b, char* type, int dur){
  interaction_t* i = GameCalloc("InitInteraction", 1, sizeof(interaction_t));

  notification n = GameNotification(type);
  cooldown_t* cd = InitCooldown(dur, n);

  i->timer = cd;
  i->source = a;
  i->target = b;


  return i;
}

void InteractionStep(interactions* p){
  hash_iter_t iter;

  HashStart(p, &iter);

  hash_slot_t* s = NULL;

  while((s = HashNext(&iter))){
    interaction_t* intact = s->value;

    if(intact->timer->elapsed >= intact->timer->duration){
      HashRemove(p, s->key);
      continue;
    }
    intact->timer->elapsed++;

  }
}

bool InteractionCheck(interactions* p, uint32_t a, uint32_t b, char* type){
  notification n = hash_str_64(type);

  hash_key_t key = hash_event(a, b, n);

  return (InteractionsGetEntry(p, key) != NULL);
}
