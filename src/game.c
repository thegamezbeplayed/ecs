#include "raylib.h"
#include "game_process.h"
#include "game_tools.h"

#include "game_systems.h"
#include "screens.h"
Camera2D camera = { 0 };
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void){
  InitGameEvents();
  GameSetState(GAME_LOADING);
}

void PreUpdate(void){
  GameProcessStep();
}

void FixedUpdate(void){
}

void PostUpdate(void){
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
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
