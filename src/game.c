#include "raylib.h"
#include "game_process.h"
#include "game_tools.h"

#include "game_define.h"
#include "game_register.h"
#include "screens.h"
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void){
  InitGameEvents();
  
  InitEntityComponentSystem();
  GameSetState(GAME_LOADING);
}

void PreUpdate(void){
  
  GameProcessStep();
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_PRE);
}

void FixedUpdate(void){
  GP.game_frames++;
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
void UpdateGameplayScreen(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_FRAME);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_FRAME);

}

// Gameplay Screen Draw logic
void BeginDraw(void){

  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW_BEGIN);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_DRAW_BEGIN);
}

void DrawGameplayScreen(void){

  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_DRAW);
}
void EndDraw(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW_END);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_SYNC), &world , UPDATE_DRAW_END);

}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
  GameProcessEnd();
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
  return 0;
//  return finishScreen;
}
