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
  InitComponents();
}

void PreUpdate(void){
  GameProcessStep();
  SystemsStep(UPDATE_PRE);
}

void FixedUpdate(void){
  SystemsStep(UPDATE_FIXED);
}

void PostUpdate(void){
  SystemsStep(UPDATE_POST);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
  SystemsStep(UPDATE_FRAME);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
  //RenderSystem(&render, &world.sprites);

  SystemsStep(UPDATE_DRAW);
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
