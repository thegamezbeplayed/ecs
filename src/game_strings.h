#ifndef __GAME_STR__
#define __GAME_STR__
#include <stdio.h>
#include "game_ui.h"
#include <errno.h>

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

static char* str_concat(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b) + 1; // +1 for null terminator
    char* result = malloc(len);
    if (!result) return NULL;

    strcpy(result, a);
    strcat(result, b);

    return result;
}

static char* sub_string(const char* base, const char* delim, int at) {
    int index = 0;

    char* str = GameCalloc("SubString", MAX_LINE_LEN, sizeof(char));
    strcpy(str, base);

    char* token = strtok(str, delim);

    while (token != NULL) {
        if (index == at) {
            char* result = GameCalloc("SubStringResult", strlen(token) + 1, sizeof(char));
            strcpy(result, token);
            GameFree("sub_string", str);
            return result;
        }

        token = strtok(NULL, delim);
        index++;
    }

    GameFree("sub_string", str);
    return NULL; // SAFE: explicitly signal not found
}

static int count_tokens(const char* str, char delim) {
    int count = 1;
    for (const char* p = str; *p; p++) {
        if (*p == delim) count++;
    }
    return count;
}

static char** split_string(const char* str, char delim, int* outCount) {
    int count = count_tokens(str, delim);
    char** result = (char**)malloc(sizeof(char*) * count);

    const char* start = str;
    int idx = 0;

    for (const char* p = str; ; p++) {
        if (*p == delim || *p == '\0') {
            int len = p - start;

            char* token = (char*)malloc(len + 1);
            memcpy(token, start, len);
            token[len] = '\0';

            result[idx++] = token;

            if (*p == '\0') break;
            start = p + 1;
        }
    }

    if (outCount) *outCount = count;
    return result;
}

static void int_to_str(int x, char* out, size_t size) {
    snprintf(out, size, "%d", x);
}


static int str_to_int(const char* str, int* out) {
    char* end;
    errno = 0;

    long val = strtol(str, &end, 10);

    if (errno != 0 || end == str) {
        return 0; // failed
    }

    *out = (int)val;
    return 1;
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

const char* StringGetAnnouncement(notification type);
#endif
