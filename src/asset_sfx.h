#ifndef __ASS_SFX__
#define __ASS_SFX__
#include "game_utils.h"

#define MAX_SONGS 4
#define EVENT_SFX_BASE   0x9000

DEFINE_EVENT_SPACE(SfxEvent, EVENT_SFX_BASE)

typedef enum{
  SFX_EVENT_NONE,
  SFX_EVENT_PLAY_SFX,
  SFX_EVENT_SONG_END,
  SFX_EVENT_SONG_FADE_OUT,
  SFX_EVENT_SONG_FADE_IN,

  SFX_EVENT_COUNT
}SfxEventID;

typedef struct{
  int     duration;
  int     elapsed;
  float   vol;
  bool    increase;
}audio_fade_t;

typedef struct{
  Music        music;
  timers_t     *timers;
  audio_fade_t *fade;
}music_track_t;

typedef struct{
  unsigned int  index;
  int           num_songs;
  unsigned int  current_index;
  float         volume;
  music_track_t track[MAX_SONGS];
}music_group_t;

typedef enum{
  SFX_ALL,
  SFX_UI,
  SFX_ACTION,
  SFX_IMPORTANT,
  SFX_DONE
}SfxGroup;

typedef enum {
  ACTION_HOVER,
  ACTION_PLACE,
  END_SFX
}SfxType;

typedef struct{
  SfxType     type;
  SfxGroup    group;
  int         num_sounds;
  const char* paths[5];
}sfx_info_t;

typedef struct{
  SfxType   type;
  int       num_sounds;
  Sound     *sounds;
}sfx_sound_t;

static sfx_info_t sfx_catalog[]={
  {ACTION_PLACE,SFX_ACTION,4,{"switch_001.ogg","switch_002.ogg","switch_003.ogg","switch_004.ogg"}},
  {ACTION_HOVER,SFX_ACTION,5,{"click_001.ogg","click_002.ogg","click_003.ogg","click_004.ogg","click_005.ogg"}},
};

typedef struct{
  int         num_types;
  float       volume;
  sfx_sound_t items[END_SFX];
}sfx_group_t;

typedef struct{
  music_group_t   *tracks;
  float           volume;
  int             current_track;
  int             concurrent_track;
  int             num_tracks;
  sfx_group_t     sfx[SFX_DONE];
  timers_t        *timers[SFX_DONE];
}audio_manager_t;
extern audio_manager_t AudioMan;

void InitAudio();
int AudioBuildMusicTracks(const char* subdir);
void AudioStep();
void AudioPlaySfx(SfxGroup group, SfxType type, int index);
void AudioPlayRandomSfx(SfxGroup group, SfxType type);
void AudioPlayMusic(int index);
bool AudioPlayNext(music_group_t* t);
void AudioMusicFade(music_track_t* t);

#endif
