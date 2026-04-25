#ifndef __GAME_PROCESS__
#define __GAME_PROCESS__

#include "screens.h" 
#include "game_types.h"
#include "game_common.h"
#include "game_utils.h"

#define MAX_INTERACTIONS 256

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
  UPDATE_DRAW,
  UPDATE_PRE,
  UPDATE_FIXED,
  UPDATE_POST,
  UPDATE_DONE
}UpdateType;

typedef enum{
  GAME_NONE,
  GAME_LOADING,
  GAME_READY,
  GAME_PAUSE,
  GAME_FINISHED,
  GAME_OVER
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

typedef struct{
  GameScreen     screen;
  int            game_frames;
  child_process_t children[SCREEN_DONE];
  GameScreen     next[SCREEN_DONE];
  GameState      state[SCREEN_DONE];//TODO each screen needs a state
  event_bus_t    *bus[SCREEN_DONE];
  int            album_id[SCREEN_DONE];
  UpdateFn       init[SCREEN_DONE];
  UpdateFn       update_steps[SCREEN_DONE][UPDATE_DONE];
  UpdateFn       finish[SCREEN_DONE];
}game_process_t;

extern game_process_t game_process;
void InitGameEvents();
void InitGameProcess();
void GameProcessStep();
void GameProcessSync(bool wait);
bool GameTransitionScreen();
void GameProcessEnd();
//===WORLD_T===>

void WorldAnnounce(EventType, Vector2 pos);

void Subscribe(EventType event, EventCallback cb, void* data);
void ScheduleEvent(EventType type, void* data, uint64_t uid, TimeFrame, int);
void GameEvent(EventType, void*, uint64_t);
void WorldInitOnce();
void WorldPreUpdate();
void WorldFixedUpdate();
void WorldPostUpdate();
void WorldRender();
Rectangle WorldRoomBounds();
static int WorldGetTime(){
  return game_process.game_frames;
}

const char* GetLevelString(void);
#endif

