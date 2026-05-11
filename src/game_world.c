#include <stdio.h>
#include <unistd.h>
#include "game_process.h"
#include "game_utils.h"
#include "game_register.h"
#include "game_define.h"
#include "scene_loader.h"
#define BUS (event_bus_t*){GP.bus}

world_t world;
game_process_t GP;

void InitEntityComponentSystem(void){
  WorldInit(&world);
  
<<<<<<< HEAD
  game_t* g = LoadGameDefine("resources/data/definitions.json");
=======
  LoadGameDefine("resources/data/definitions.json");
  RegisterComponentData(&world);
  RegisterSystemData(&world);
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594

  if(!g)
    return;

  GameInitPrefabs(&world, g);
  for(int i = 0; i < world.num_sys; i++){
    if(world.systems[i].init)
      world.systems[i].init(&world);
  }

<<<<<<< HEAD
//  GameSpawn(&world, g);
  UnloadGameDefine(g);
=======
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
  Scene* test = GameCalloc("InitEntityComponentSystem", 1, sizeof(Scene));

  bool scene = SceneLoadByIndex(0, test);

  TraceLog(LOG_INFO," ==== LOADED SCENE %s %s", test->name, test->display_name);
<<<<<<< HEAD

  SceneSetup(&world, test);
=======
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
}

void Subscribe(uint64_t event, EventCallback cb, void* data){
  event_sub_t* sub = EventSubscribe(BUS, event , cb, data);
  sub->eid = -1;
}

void SubscribeEntity(uint64_t event, EventCallback cb, void* data, int id){
  event_sub_t* sub = EventSubscribe(BUS, event, cb, data);
  sub->eid = id;
}

void TargetSubscribe(uint64_t event, EventCallback cb, void* data, int id){
  event_sub_t* sub = EventSubscribe(BUS, event, cb, data);
  sub->eid = id;
}

void ScheduleEvent(uint64_t event, void* data, uint64_t uid, TimeFrame tf, int step){
  switch(tf){
    case TF_TURN:
//      step += WorldGetTurn();
      break;
    case TF_UPDATE:
      step += WorldGetTime();
      break;
    default:
      return;
      break;
  }

  event_t* ev = InitEvent(GP.notifications, event, data, uid);

  ev->timing = tf;
  ev->scheduled = step;

  EventSchedule(BUS, ev);
}

void GameEvent(uint64_t event, void* data, uint64_t uid){
  if(BUS->count == 0)
    return;

  event_t* ev = InitEvent(GP.notifications, event, data, uid);
  EventEmit(BUS, ev);
  GameFree("GameEvent", ev);
}

void GameSetState(GameState state){
  if(GP.state[SCREEN_GAMEPLAY] == state)
    return;

  TraceLog(LOG_INFO, "==== GAME STATE ====\n set to %i", state);
  GP.state[SCREEN_GAMEPLAY] = state;
  GameEvent(GameEvent_ToNotif(GAME_EVENT_STATE), &world , state);

  if(GP.cb[state])
    GP.cb[state](state);
}

void InitGameProcess(){
  for(int s = 0; s<SCREEN_DONE; s++){
    GP.album_id[s] = -1;
    for(int u = 0; u<UPDATE_DONE;u++){
      GP.update_steps[s][u] = DO_NOTHING;
    
    }
    GP.children[s].process= PROCESS_NONE;
    for(int p = 0; p < PROCESS_DONE; p++)
      GP.children[s].state[p]=GAME_NONE;
  }

  GP.cb[GAME_LOADING] = GameStepState;
  GP.cb[GAME_READY] = GameStepState;
   
  GP.next[SCREEN_LOGO] = SCREEN_GAMEPLAY;
  GP.phase[SCREEN_LOGO][GAME_LOADING] = InitLogoScreen;
  GP.phase[SCREEN_LOGO][GAME_FINISHED] = UnloadLogoScreen;
  GP.update_steps[SCREEN_LOGO][UPDATE_DRAW] = DrawLogoScreen;
  GP.update_steps[SCREEN_LOGO][UPDATE_FRAME] = UpdateLogoScreen;
 
  GP.next[SCREEN_TITLE] = SCREEN_GAMEPLAY;
  GP.phase[SCREEN_TITLE][GAME_LOADING] = InitTitleScreen;
  GP.phase[SCREEN_TITLE][GAME_FINISHED] = UnloadTitleScreen;
  GP.update_steps[SCREEN_TITLE][UPDATE_DRAW] = DrawTitleScreen;
  GP.update_steps[SCREEN_TITLE][UPDATE_FRAME] = UpdateTitleScreen;

  GP.next[SCREEN_GAMEPLAY] = SCREEN_ENDING;
  GP.phase[SCREEN_GAMEPLAY][GAME_LOADING] = InitGameplayScreen;
  GP.phase[SCREEN_GAMEPLAY][GAME_FINISHED] = UnloadGameplayScreen;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_FIXED] = FixedUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_PRE] = PreUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW_BEGIN] = BeginDraw;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW_END] = EndDraw;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_FRAME] = UpdateGameplayScreen;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_POST] = PostUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_FINAL] = FinalUpdate;
  //GP.album_id[SCREEN_GAMEPLAY] = AudioBuildMusicTracks("bingbong");

  //GP.children[SCREEN_GAMEPLAY].update_steps[PROCESS_LEVEL][UPDATE_FIXED] = LevelFixedUpdate; 
  GP.children[PROCESS_LEVEL].process = PROCESS_LEVEL;
  GP.next[SCREEN_ENDING] = SCREEN_TITLE;
  GP.phase[SCREEN_ENDING][GAME_LOADING] = InitEndScreen;
  GP.phase[SCREEN_ENDING][GAME_FINISHED] = UnloadEndScreen;
  GP.update_steps[SCREEN_ENDING][UPDATE_DRAW] = DrawEndScreen;
  GP.update_steps[SCREEN_ENDING][UPDATE_FRAME] = UpdateEndScreen;
<<<<<<< HEAD
  
  GP.screen = SCREEN_LOGO;
=======

  //GP.screen = SCREEN_TITLE;
  GP.screen = SCREEN_LOGO;
  //GP.state[SCREEN_GAMEPLAY] = GAME_LOADING;

 
>>>>>>> 9a78f7bda89d2d6c55d3d73a5974c422944a4594
  GP.phase[SCREEN_LOGO][GAME_LOADING]();
}

void InitGameEvents(){
  GP.children[SCREEN_GAMEPLAY].process = PROCESS_LEVEL;
  GP.game_frames = 0; 

  GP.bus = InitEventBus(128);
  GP.notifications = InitNotifications(64);
}

bool GameTransitionScreen(){
  GameScreen current = GP.screen;
  GameScreen prepare = GP.next[current];
  if(GP.state[current] >= GAME_FINISHED)
    return false;
  GP.state[current] = GAME_FINISHED;
  GP.phase[current][GAME_FINISHED]();
  GP.screen = prepare;
  GP.phase[prepare][GAME_LOADING]();

  //AudioPlayMusic(GP.album_id[prepare]);

  return true;
}

void GameProcessStep(){
  if(BUS)
    EventBusStep(BUS);
}

void GameProcessSync(bool wait){
  if(GP.state[GP.screen] > GAME_RUNNING){
    GP.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW]();
    return;
  }
  
  for(int i = 0; i < UPDATE_DONE;i++){
    if(i > UPDATE_DRAW_END && wait)
      return;

    GP.update_steps[GP.screen][i]();
  }

  for(int i = 0; i < PROCESS_DONE;i++){
    if(GP.children[GP.screen].process==PROCESS_NONE)
      continue;
    child_process_t* kids = &GP.children[GP.screen];
    for(int j = 0; j < UPDATE_DONE; j++)
      if(kids->update_steps[i][j]!=NULL)
        kids->update_steps[i][j]();
  }
}

void GameStepState(GameState s){
  if(s < GAME_DONE){
    TraceLog(LOG_INFO, "=== GAME STATE ===\n step state to %i", s+1);
    GameSetState(s+1);

  }
}

void GameProcessEnd(){
}
