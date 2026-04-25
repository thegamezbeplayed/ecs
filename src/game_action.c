#include "game_control.h"
#include "game_types.h"

turn_action_manager_t ActionMan;

int ActionCompareImpDsc(const void* a, const void* b){
  const action_t* A = (const action_t*)a;
  const action_t* B = (const action_t*)b;
  
  if (A->score > B->score) return -1;
  if (A->score < B->score) return  1;

  if (A->weight > B->weight) return -1;
  if (A->weight < B->weight) return  1;

  return 0;
}

int ActionCompareInitDsc(const void* a, const void* b){
  const action_t* A = *(const action_t* const*)a;
  const action_t* B = *(const action_t* const*)b;

  if(A->initiative > B->initiative) return -1;
  if(A->initiative < B->initiative) return  1;

  return 0;
}

void ActionQueueSortByInitiative(action_queue_t* q){
  if (!q || q->count <= 1)
    return;

  qsort(q->entries,
      q->count,
      sizeof(action_t),
      ActionCompareInitDsc);

}

void ActionQueueSortByImportance(action_queue_t* q){
  if (!q || q->count <= 1)
    return;

  qsort(q->entries,
      q->count,
      sizeof(action_t),
      ActionCompareImpDsc);

}

void InitActionRound(TurnPhase phase, int cap){
  ActionMan.round[phase] = (action_round_t){
    .cap = cap,
      .phase = phase,

  }; 
}

void InitActionManager(void){

  for (int i = 0; i < TURN_ALL; i++)
    InitActionRound(i, MAX_PHASE_ACTIONS);

  ActionMan.phase = TURN_MAIN;
  ActionMan.next = TURN_END;
  //WorldSubscribe(EVENT_PLAYER_INPUT, OnPlayerAction, &ActionMan); 
}

bool ActionManagerInsert(action_t* a, TurnPhase phase){
  if(ActionMan.round[phase].count >= MAX_PHASE_ACTIONS)
    return false;

  ActionMan.round[phase].entries[ ActionMan.round[phase].count++] = a;

  return true;
}

void ActionPrune(action_queue_t* q, int i) {
  if(q->count > 1)
    q->entries[i] = q->entries[--q->count];
  else
    q->count--;
}

bool ActionSetStatus(action_t* a, ActionStatus status){
  if(a->status == status)
    return false;

  a->status = status;
  return true;
}

void ActionManagerBuildQueue(TurnPhase phase){
}

ActionStatus ActionAttack(action_t* a){
  ActionStatus s = a->status;

  bool prepared = false;
  
  if(!prepared){
    s = ACT_STATUS_BAD_ATTACK;
  }

  ActionSetStatus(a, s);
  return a->status;
}

ActionStatus ActionInteract(action_t* a){
  InteractResult res = IR_NONE;

  return a->status;
}

ActionStatus ActionValidate(action_t* a){
  switch(a->type){
    default: 
    break;
  }

  return ACT_STATUS_NONE;
}

ActionStatus ActionMove(action_t* a){
}

ActionStatus ActionRun(action_t* a){
  ActionStatus s = ACT_STATUS_RUNNING;
}

void ActionRoundSort(TurnPhase phase, int count){
  if (count <= 1)
    return;

  qsort(ActionMan.round[phase].entries,
      count,
      sizeof(action_t*),
      ActionCompareInitDsc);

}   

void ActionManagerRunQueue(TurnPhase phase){
  int count = ActionMan.round[phase].count;

  ActionRoundSort(phase,count);

  for (int i = 0; i < count; i++){
    action_t* a = ActionMan.round[phase].entries[i];
    ActionStatus validated = ActionValidate(a);
    if(validated < ACT_STATUS_ERROR)
      ActionSetStatus(a, ActionRun(a));
    else
      ActionSetStatus(a, validated);

    switch(a->status){
      default:
        break;

    }
  }
}

bool ActionTurnStep(void){
  if(ActionMan.round[ActionMan.phase].status != ACT_STATUS_DONE)
    return false;

  ActionMan.round[ActionMan.phase].status = ACT_STATUS_NONE;

  if(ActionMan.next < TURN_END){
    ActionMan.phase = ActionMan.next;
    ActionMan.next++;

    return true;
  }
  else{
    ActionMan.phase = ActionMan.next;
    ActionMan.next = TURN_MAIN;
    return true;
  }

 TraceLog(LOG_WARNING,"===== ACTION MANAGER TURN =====\nShouldnt reach here...");
 return false; 

}

void ActionManOnStatus(TurnPhase phase, ActionStatus s){
  switch(s){
    case ACT_STATUS_DONE:
      ActionTurnStep();
      break;
  }
}

bool ActionManSetStatus(TurnPhase phase, ActionStatus next){
  if(ActionMan.round[phase].status == next)
    return false;

  if(ActionMan.round[phase].status > next)
    return false;

  ActionMan.round[phase].status = next;

  ActionManOnStatus(phase, next);
  return true;
}

void ActionManagerEndRound(TurnPhase phase){
}

void ActionManagerEndTurn(){

}

void ActionPhaseStep(TurnPhase phase){
}

void ActionManagerSync(void){
    ActionPhaseStep(ActionMan.phase);
}

action_t* InitAction(ActionType type, uint64_t gouid, int weight){

}

BehaviorStatus ActionExecute(ActionType t, action_t* a){
  if(!a)
    return BEHAVIOR_FAILURE;
}

action_queue_t* InitActionQueue(int cap){

  action_queue_t* q = GameCalloc("InitActionQueue", 1, sizeof(action_queue_t));

  *q = (action_queue_t) {
    .cap = cap,
  };

  q->entries = GameCalloc("InitActionQueue entries", q->cap, sizeof(action_t));

  return q;
}

action_t* ActionForPhase(action_queue_t* q, TurnPhase phase){
  if(q->charges <1)
    return NULL;

  for(int i = 0; i < q->count; i++){
    action_t* a = &q->entries[i];
    if(a->status > ACT_STATUS_QUEUED)
      continue;
    q->charges--;
    q->status = ACT_STATUS_NEXT;
    return &q->entries[i];
  }

  return NULL;
}

bool ActionQueueEnsureCap(action_queue_t *q){
  if(q->count < q->cap)
    return true;

  if(q->cap >= MAX_ACTIONS)
    return false;

  q->cap+=4;

  q->entries = GameRealloc("ActionQueueEnsureCap", q->entries, q->cap * sizeof(action_t));

  return true;
}

action_t* ActionFindByID(action_queue_t *q, action_t* t){
  for(int i = 0; i < q->count; i++){
    if(q->entries[i].id == t->id)
      return &q->entries[i];
  }

  return NULL;
}

ActionStatus AddAction(action_queue_t *q, action_t* t){
  if (ActionSetStatus(t,ACT_STATUS_QUEUED)) 
    q->entries[q->count++] = *t;

  return t->status;
}

ActionStatus QueueAction(action_queue_t *q, action_t* t){
  if(q->status >= ACT_STATUS_QUEUED)
    return ACT_STATUS_WAIT;

  action_t* dupe = ActionFindByID(q,t);

  if(dupe){
    dupe->weight = t->weight;

    return dupe->status;

  }

  if(!ActionQueueEnsureCap(q))
    return ACT_STATUS_FULL;
  
  return AddAction(q, t);
}


bool ActionSync(action_t* a){
  if(!a || a->status >= ACT_STATUS_TAKEN)
    return false;


  return true;
}

void ActionQueueSync(action_queue_t* q){

  for (int i = 0; i < q->count; i++){
    action_t* a = &q->entries[i];
    if(ActionSync(&q->entries[i]))
      continue;

    ActionPrune(q, i);
  }

  if(q->count < 2)
    return;

  ActionQueueSortByImportance(q);
}

int ActionSlotCompareDesc(const void* a, const void* b){
  // Primary: threat
  const int A = *(const int*)a;
  const int B = *(const int*)b;
  
  if (A > B) return -1;
  if (B < B) return  1;

  return 0;
}
