#include "util_tools.h"
#include "system_define.h"

void LookSystemSink(int cap){
  HashInit(&SYSTEM_SINK, next_pow2_int(cap));
}

void LookAddSink(const char* str, ObserverCB fn){
  hash_key_t key = hash_str_64(str);

  TraceLog(LOG_INFO, "=== ADDED SYSTEM SINK %s ===", str);
  HashPut(&SYSTEM_SINK, key, fn);
}

ObserverCB LookupSystemSink(const char* str){
  hash_key_t key = hash_str_64(str);

  return HashGet(&SYSTEM_SINK, key);
}

