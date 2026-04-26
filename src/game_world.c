#include <stdio.h>
#include <unistd.h>
#include "game_process.h"
#include "game_utils.h"

#define BUS (event_bus_t*){game_process.bus[game_process.screen]}

game_process_t game_process;

void Notification(notification hash, EventCallback cb, void* data){
  event_sub_t* sub = EventSubscribe(BUS, hash, cb, data);

}

void Subscribe(char* event, EventCallback cb, void* data){
  notification hash = hash_str_64(event);
  event_sub_t* sub = EventSubscribe(BUS, hash, cb, data);
  sub->eid = -1;
}

void TargetSubscribe(notification hash, EventCallback cb, void* data, int id){
  event_sub_t* sub = EventSubscribe(BUS, hash, cb, data);
  sub->eid = id;
}

void ScheduleEvent(char* event, void* data, uint64_t uid, TimeFrame tf, int step){
  switch(tf){
    case TF_TURN:
//      step += WorldGetTurn();
      break;
    case TF_UPDATE:
  //    step += WorldGetTime();
      break;
    default:
      return;
      break;
  }

  event_t* ev = InitEvent(game_process.notifications, event, data, uid);

    ev->timing = tf,
    ev->scheduled = step,

  EventSchedule(BUS, ev);
}

void GameEvent(char* event, void* data, uint64_t uid){
  event_t* ev = InitEvent(game_process.notifications, event, data, uid);

  if(BUS->count)
    EventEmit(BUS, ev);
}

bool TogglePause(void){
  if(game_process.state[SCREEN_GAMEPLAY] == GAME_READY)
    GameSetState(GAME_PAUSE);
  else if (game_process.state[SCREEN_GAMEPLAY] == GAME_PAUSE)
    GameSetState(GAME_READY);

  return true;
}

void GameSetState(GameState state){
  if(game_process.screen != SCREEN_GAMEPLAY)
    return;

  game_process.state[SCREEN_GAMEPLAY] = state;
}

void WorldInitOnce(){
}

void WorldPreUpdate(){
}

void WorldFixedUpdate(){
  
}

void WorldPostUpdate(){
}

void InitGameProcess(){
  for(int s = 0; s<SCREEN_DONE; s++){
    game_process.album_id[s] = -1;
    for(int u = 0; u<UPDATE_DONE;u++){
      game_process.update_steps[s][u] = DO_NOTHING;
    
    }
    game_process.children[s].process= PROCESS_NONE;
    for(int p = 0; p < PROCESS_DONE; p++)
      game_process.children[s].state[p]=GAME_NONE;
  }

  game_process.next[SCREEN_TITLE] = SCREEN_GAMEPLAY;
  game_process.init[SCREEN_TITLE] = InitTitleScreen;
  game_process.finish[SCREEN_TITLE] = UnloadTitleScreen;
  game_process.update_steps[SCREEN_TITLE][UPDATE_DRAW] = DrawTitleScreen;
  game_process.update_steps[SCREEN_TITLE][UPDATE_FRAME] = UpdateTitleScreen;

  game_process.next[SCREEN_GAMEPLAY] = SCREEN_ENDING;
  game_process.init[SCREEN_GAMEPLAY] = InitGameplayScreen;
  game_process.finish[SCREEN_GAMEPLAY] = UnloadGameplayScreen;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_FIXED] = FixedUpdate;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_PRE] = PreUpdate;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW] = DrawGameplayScreen;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_FRAME] = UpdateGameplayScreen;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_POST] = PostUpdate;
  //game_process.album_id[SCREEN_GAMEPLAY] = AudioBuildMusicTracks("bingbong");

  //game_process.children[SCREEN_GAMEPLAY].update_steps[PROCESS_LEVEL][UPDATE_FIXED] = LevelFixedUpdate; 
  game_process.children[PROCESS_LEVEL].process = PROCESS_LEVEL;
  game_process.next[SCREEN_ENDING] = SCREEN_TITLE;
  game_process.init[SCREEN_ENDING] = InitEndScreen;
  game_process.finish[SCREEN_ENDING] = UnloadEndScreen;
  game_process.update_steps[SCREEN_ENDING][UPDATE_DRAW] = DrawEndScreen;
  game_process.update_steps[SCREEN_ENDING][UPDATE_FRAME] = UpdateEndScreen;

  //game_process.screen = SCREEN_TITLE;
  game_process.screen = SCREEN_GAMEPLAY;
  //game_process.state[SCREEN_GAMEPLAY] = GAME_LOADING;
  game_process.init[SCREEN_GAMEPLAY]();

}

void InitGameEvents(){
  game_process.children[SCREEN_GAMEPLAY].process = PROCESS_LEVEL;
  game_process.game_frames = 0; 

  game_process.bus[SCREEN_GAMEPLAY] = InitEventBus(64);
  game_process.notifications = InitNotifications(64);
}

bool GameTransitionScreen(){
  GameScreen current = game_process.screen;
  GameScreen prepare = game_process.next[current];
  if(game_process.state[current] >= GAME_FINISHED)
    return false;
  game_process.init[prepare]();
  game_process.state[current] = GAME_FINISHED;
  game_process.finish[current]();
  game_process.screen = prepare;
  game_process.state[prepare] = GAME_LOADING;
  //AudioPlayMusic(game_process.album_id[prepare]);

  return true;
}

void GameProcessStep(){
  if(BUS)
    EventBusStep(BUS);
}

void GameProcessSync(bool wait){
  if(game_process.state[game_process.screen] > GAME_READY){

    game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW]();
  
    return;
  }
  
  for(int i = 0; i <UPDATE_DONE;i++){
    if(i > UPDATE_DRAW && wait)
      return;
    game_process.update_steps[game_process.screen][i]();
  }

  for(int i = 0; i < PROCESS_DONE;i++){
    if(game_process.children[game_process.screen].process==PROCESS_NONE)
      continue;
    child_process_t* kids = &game_process.children[game_process.screen];
    for(int j = 0; j < UPDATE_DONE; j++)
      if(kids->update_steps[i][j]!=NULL)
        kids->update_steps[i][j]();
  }
}

void GameProcessEnd(){
}
