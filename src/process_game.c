#include "process_define.h"
#include "asset_sfx.h"

game_process_t GP;

bool GameSetScreen(GameScreen s){
  if(GP.screen == s)
    return false;

  TraceLog(LOG_INFO, "=== GAME SCREEN SET TO %i ===", s);
  GP.screen = s;

  return GameSetState(GAME_LOADING);
}

bool GameSetState(GameState state){
  if(GP.state[GP.screen] == state)
    return false;

  GP.state[GP.screen] = state;
  GP.phase[GP.screen][state]();

  if(GP.screen == SCREEN_GAMEPLAY){ 
    GameOnStateChange(state);
    if(GP.cb[state])
      GP.cb[state](state);
  }
  return true;
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

  GP.next[SCREEN_LOGO] = SCREEN_TITLE;
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
  GP.phase[SCREEN_GAMEPLAY][GAME_LOADING] = InitGameplay;
  GP.phase[SCREEN_GAMEPLAY][GAME_READY] = ReadyGameplay;
  GP.phase[SCREEN_GAMEPLAY][GAME_RUNNING] = RunGameplay;
  GP.phase[SCREEN_GAMEPLAY][GAME_FINISHED] = UnloadGameplay;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_FIXED] = FixedUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_PRE] = PreUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW_BEGIN] = BeginDraw;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW] = UpdateDraw;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW_END] = EndDraw;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_FRAME] = FrameUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_POST] = PostUpdate;
  GP.update_steps[SCREEN_GAMEPLAY][UPDATE_FINAL] = FinalUpdate;
  GP.album_id[SCREEN_GAMEPLAY] = AudioBuildMusicTracks("bingbong");

  //GP.children[SCREEN_GAMEPLAY].update_steps[PROCESS_LEVEL][UPDATE_FIXED] = LevelFixedUpdate; 
  GP.children[PROCESS_LEVEL].process = PROCESS_LEVEL;
  GP.next[SCREEN_ENDING] = SCREEN_TITLE;
  GP.phase[SCREEN_ENDING][GAME_LOADING] = InitEndScreen;
  GP.phase[SCREEN_ENDING][GAME_FINISHED] = UnloadEndScreen;
  GP.update_steps[SCREEN_ENDING][UPDATE_DRAW] = DrawEndScreen;
  GP.update_steps[SCREEN_ENDING][UPDATE_FRAME] = UpdateEndScreen;
}

bool GameTransitionScreen(){
  GameScreen current = GP.screen;
  GameScreen prepare = GP.next[current];
  if(!GameSetState(GAME_FINISHED))
    return false;

  GameSetScreen(prepare);

  AudioPlayMusic(GP.album_id[prepare]);

  return true;
}

void GameProcessStep(){
  if(GameBus())
    EventBusStep(GameBus());
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
  if(s < GAME_DONE)
    GameSetState(s+1);

}
