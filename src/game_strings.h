#ifndef __GAME_STR__
#define __GAME_STR__
#include <stdio.h>
#include "game_ui.h"

void StringPrependPadding(char* s, size_t padding);
void StringAppendPadding(char* s, size_t padding);
void RepeatChar(char* out, size_t cap, char c, int times);

static void replace_char(char* str, char find, char replace) {
  if (!str) return;
  for (int i = 0; str[i]; i++) {
    if (str[i] == find)
      str[i] = replace;
  }
}

static char* SubString(char* base, char* delim, int at){
  int index = 0;

  char* str = GameCalloc("SubString", MAX_LINE_LEN, sizeof(char));

  strcpy(str, base);
  char *token = strtok(str, delim);

  while (token != NULL) {
    if (index == at) {
      return token;
      break;
    }

    token = strtok(NULL, delim);
    index++;
  }
}

static char* StringSplit(char* str, char delimiter){
  char* token = strtok(str, &delimiter);

  return token;
}
void StringBounds(Rectangle *b, char* buff);

typedef struct{
  const char   str[MAX_NAME_LEN];
  int         weight;
}announce_string_t;

typedef struct{
  int               count;
  announce_string_t pool[10];
  choice_pool_t     *picker;
}announce_string_p;

static  announce_string_p ANNOUNCE_STRINGS[EVENT_NONE] = {
};

const char* StringGetAnnouncement(EventType type);
#endif
