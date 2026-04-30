#include "raylib.h"
#include "game_process.h"
#include "game_tools.h"

#include "game_register.h"
#include "game_define.h"
#include "screens.h"
Camera2D camera = { 0 };
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

void InitGameData(void){
  RegisterComponentData(&world);
  RegisterSystemData(&world);
  RegisterEntityData(&world);
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void){
  InitGameEvents();
  WorldInit(&world, NUM_SYS);
  InitGameData();
  GameSetState(GAME_LOADING);
}

void PreUpdate(void){
  
  GameProcessStep();
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_PRE);
}

void FixedUpdate(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_FIXED);
}

void PostUpdate(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_POST);
 
} 
 

// Gameplay Screen Update logic
void UpdateGameplayScreen(void){
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_FRAME);

}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void){
  BeginDrawing();
  ClearBackground(BLACK);

  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW);

  DrawFPS(10, 10);
  EndDrawing();

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
