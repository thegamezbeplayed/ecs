#include <time.h>
#include "app_resource.h"
#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions
#include "game_ui.h"
#include "asset_sfx.h"
#include "game_process.h"
#include "scene_loader.h"

#include "rlgl.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

LoadQueue loader = {0};
float screenWidth = 1920.0f;
float screenHeight = 1080.0f; 
double currentTime = 0.0;           // Current time measure
double updateDrawTime = 0.0;        // Update + Draw time
double previousTime = 0.0;    // Previous time measure
double waitTime = 0.0;              // Wait time (if target fps required)
float deltaTime = 0.0f;             // Frame time (Update + Draw + Wait time)

int tarFPS = 60;
float progress = .0f;

void UpdateDrawFrame(void);          // Update and draw one frame
static void ChangeToScreen(GameScreen screen);     // Change to screen, no transition effect

static void TransitionToScreen(GameScreen screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)
bool wantQuit = false;
bool resLoaded = false;
void* AppBackgroundLoader(void* arg);
bool IsLoadingFinished(void)
{
  return atomic_load(&loader.finished);
}

float GetLoadingProgress(void)
{
  for (int i = 0; i < loader.count; i++){
    LoadJob* job = &loader.jobs[i];

    if (atomic_load(&job->ready) && !atomic_load(&job->done))
    {
      if (IsImageValid(job->image))
      {
        job->texture = LoadTextureFromImage(job->image);
        UnloadImage(job->image);                    // free CPU memory

        if (job->dest != NULL)
        {
          *job->dest = job->texture;       // Assign to original variable!
        }

        atomic_store(&job->done, true);
      }
    }

  }
  if (loader.count == 0) return 1.0f;
  int done = atomic_load(&loader.load_count);
  return (float)done / loader.count;
}

int main(void)
{
  GameSetFrameRate(tarFPS);
  srand((unsigned int)time(NULL));  // seed once using current time

  InitWindow(screenWidth,screenHeight, "raylib game template");
  InitAudioDevice();      // Initialize audio device

  SpriteLoadSplash("resources/splash.png", VEC_NEW(screenWidth,screenHeight));
  InitAudio();  
  InitGameProcess();

  SceneInit(&loader);

  pthread_t t;
  pthread_create(&t, NULL, AppBackgroundLoader, &loader);
  pthread_detach(t); 
  InitResources();
  InitUI();

  //SetTargetFPS(60);
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else

  //SetExitKey(KEY_NULL);
  // Main game loop
  while (!WindowShouldClose() && !wantQuit)    // Detect window close button or ESC key
  {
    if(!resLoaded )
    {
      progress = GetLoadingProgress();

      if (IsLoadingFinished()){
        resLoaded = true;
        GameTransitionScreen();
      }
    }

    bool wait = true;

    currentTime = GetTime();
    updateDrawTime = currentTime - previousTime;

    if(tarFPS>0){
      waitTime += updateDrawTime;
      if (waitTime < 1.0f/tarFPS)
      {
        //WaitTime((float)waitTime);
        currentTime = GetTime();
        deltaTime = (float)(currentTime - previousTime);
      }
      else{
        wait = false;        
        waitTime = 0;
      }
    }
    else
      deltaTime = (float)updateDrawTime;    // Framerate could be variable

    previousTime = currentTime;

    GameProcessSync(wait);
  }
#endif
  // Unload global data loaded
  //UnloadFont(font);
  //  UnloadMusicStream(music);

  CloseAudioDevice();     // Close audio context

  CloseWindow();          // Close window and OpenGL context
                          //--------------------------------------------------------------------------------------

  return 0;
}

void UpdateDrawFrame(void){
  if (IsKeyPressed(KEY_ENTER))// || IsGestureDetected(GESTURE_TAP))
  {
    GameTransitionScreen();
  }

  previousTime = currentTime;
  GameProcessSync(false);
}

void* AppBackgroundLoader(void* arg)
{
  LoadQueue* q = (LoadQueue*)arg;
  atomic_store(&q->load_count, 0);
  atomic_store(&q->finished, false);

  for (int i = 0; i < q->count; i++)
  {
    LoadJob* job = &q->jobs[i];

    job->image = LoadImage(job->path);        // Safe on any thread
    atomic_store(&job->ready, true);
  }
  SceneLoadResources();
  pthread_mutex_lock(&q->mutex);
  atomic_store(&q->finished, true);
  pthread_mutex_unlock(&q->mutex);
  return NULL;
}
