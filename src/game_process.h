#ifndef __GAME_PROCESS__
#define __GAME_PROCESS__

#include "screens.h" 
#include "game_types.h"
#include "game_common.h"
#include "game_utils.h"

#define MAX_INTERACTIONS 256
DEFINE_EVENT_SPACE(GameEvent, EVENT_GAME_BASE)

extern Font font;
static int fixedFPS = 60;

typedef void (*UpdateFn)(void);

typedef enum{
  PROCESS_NONE,
  PROCESS_LEVEL,
  PROCESS_DONE
}ProcessType;

typedef enum{
  UPDATE_FRAME,//update running at full fps
  UPDATE_DRAW_BEGIN,
  UPDATE_DRAW,
  UPDATE_DRAW_END,
  UPDATE_PRE,
  UPDATE_FIXED,
  UPDATE_POST,
  UPDATE_DONE
}UpdateType;

typedef enum{
  GAME_NONE,
  GAME_LOADING,
  GAME_READY,
  GAME_RUNNING,
  GAME_PAUSE,
  GAME_FINISHED,
  GAME_OVER,
  GAME_DONE
}GameState;

void GameSetState(GameState state);

typedef struct{
  ProcessType process;
  GameState   state[PROCESS_DONE];
  timer_t    *events;
  UpdateFn    init[PROCESS_DONE];
  UpdateFn    update_steps[PROCESS_DONE][UPDATE_DONE];
  UpdateFn    finish[PROCESS_DONE];
}child_process_t;

typedef void (*GameStateCB)(GameState);
void GameStepState(GameState s);

typedef struct{
  GameScreen           screen;
  int                  game_frames;
  child_process_t      children[SCREEN_DONE];
  GameScreen           next[SCREEN_DONE];
  GameState            state[SCREEN_DONE];
  GameStateCB          cb[GAME_DONE];
  notification_pool_t* notifications;
  interactions         interactions;
  event_bus_t          *bus[SCREEN_DONE];
  int                  album_id[SCREEN_DONE];
  UpdateFn             phase[SCREEN_DONE][GAME_DONE];
  UpdateFn             update_steps[SCREEN_DONE][UPDATE_DONE];
}game_process_t;
extern game_process_t GP;

void InitGameEvents();
void InitGameProcess();
void GameProcessStep();
void GameProcessSync(bool wait);
bool GameTransitionScreen();
void GameProcessEnd();
//===WORLD_T===>

void WorldAnnounce(notification, Vector2 pos);
void SubscribeEntity(uint64_t, EventCallback, void*, int);
void TargetSubscribe(uint64_t, EventCallback cb, void* data, int);
void Subscribe(uint64_t, EventCallback cb, void* data);
void ScheduleEvent(uint64_t, void* data, uint64_t uid, TimeFrame, int);
void GameEvent(uint64_t, void*, uint64_t);
void WorldInitOnce();
void WorldPreUpdate();
void WorldFixedUpdate();
void WorldPostUpdate();
void WorldRender();
Rectangle WorldRoomBounds();
static int WorldGetTime(){
  return GP.game_frames;
}

static notification GameNotification(char* str){
  notification_t* n = RegisterNotification(GP.notifications, str);
}

static void GameInteraction(interaction_t* entry){

  RegisterInteraction(&GP.interactions, entry);
}
static bool GameCheckInteraction(uint32_t a, uint32_t b, char* type){
  return InteractionCheck(&GP.interactions, a, b, type);
}

const char* GetLevelString(void);

static uint64_t MakeGUID(char* str, int index){
  uint64_t hash = hash_str_64(str);

  uint64_t uid = hash_combine_64(hash, hash_64_from_int(index));

  return hash_combine_64(uid, hash_64_from_int(WorldGetTime()));
}

void InitEntityComponentSystem(void);

#endif

