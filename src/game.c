#include "raylib.h"
#include "process_event.h"
#include "process_load.h"

#include "game_register.h"
#include "asset_sfx.h"
#include "screens.h"
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplay(void){
  InitGameEvents();
  InitGameWorld();
  GameLoad();
}

void ReadyGameplay(void){
}

void RunGameplay(void){}

void StartUpdate(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_START);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_START);
}

void PreUpdate(void){
  GameProcessStep();
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_PRE);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_PRE);
}

void FixedUpdate(void){
  GP.game_frames++;
  AudioStep();
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_FIXED);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_FIXED);
}

void PostUpdate(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_POST);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_POST);
} 

void FinalUpdate(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_FINAL);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_FINAL);
}

// Gameplay Screen Update logic
void FrameUpdate(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_FRAME);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_FRAME);

}

// Gameplay Screen Draw logic
void BeginDraw(void){

  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW_BEGIN);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_DRAW_BEGIN);
}

void UpdateDraw(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_DRAW);
}
void EndDraw(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW_END);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_DRAW_END);

}

// Gameplay Screen Unload logic
void UnloadGameplay(void)
{
  GameUnloadEvents();
}

// Gameplay Screen should finish?
int FinishGameplay(void)
{
  return 0;
//  return finishScreen;
}
