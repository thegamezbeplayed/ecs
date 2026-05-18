#ifndef __PROC_DEF__
#define __PROC_DEF__

#include "screens.h" 
#include "game_types.h"
#include "game_common.h"
#include "game_utils.h"

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
  UPDATE_FINAL,
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

bool GameSetScreen(GameScreen s);
bool GameSetState(GameState state);

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
  int                  fps, game_frames;
  child_process_t      children[SCREEN_DONE];
  GameScreen           next[SCREEN_DONE];
  GameState            state[SCREEN_DONE];
  GameStateCB          cb[GAME_DONE];
  int                  album_id[SCREEN_DONE];
  UpdateFn             phase[SCREEN_DONE][GAME_DONE];
  UpdateFn             update_steps[SCREEN_DONE][UPDATE_DONE];
}game_process_t;
extern game_process_t GP;

void InitGameEvents();
event_bus_t* GameBus(void);
void GameOnStateChange(GameState);
void InitGameProcess();
void GameProcessStep();
void GameProcessSync(bool wait);
bool GameTransitionScreen();
void GameProcessEnd();


void InitEntityComponentSystem(void);

static void GameSetFrameRate(int rate){
  GP.fps = rate;
}
static int WorldGetTime(){
  return GP.game_frames;
}

static int GameGetFrameRate(void){ return GP.fps;}
#endif
