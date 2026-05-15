#ifndef __GAME_HELPERS__
#define __GAME_HELPERS__

#include "game_define.h"

static bool array_contains_int(const int* arr, int count, int value)
{
    if (arr == NULL || count <= 0)
        return false;

    for (int i = 0; i < count; ++i)
    {
        if (arr[i] == value)
            return true;
    }
    return false;
}

static bool COMPARE_ALL_BOOL(bool *all, int count){
  for (int i = 0; i < count; i++)
    if(!all[i])
      return false;

  return true;
}

static Color ColorFromHexString(const char* hex)
{
  if (!hex || strlen(hex) < 8) return BLACK;

  unsigned int hexValue = (unsigned int)strtoul(hex, NULL, 16);
  return GetColor(hexValue);        // raylib built-in function
}
#endif
