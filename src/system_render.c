#include "game_systems.h"

void RenderLoad(world_t* w, Entity e){

}

void RenderBegin(world_t* w, Entity e){
  BeginDrawing();
  ClearBackground(BLACK);
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STEP), &world , UPDATE_DRAW);

}

void RenderEnd(world_t* w, Entity e){
  DrawFPS(10, 10);
  EndDrawing();

}
