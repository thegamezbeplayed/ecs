#include "util_tools.h"

static int TOTAL_SIZE = 0;

void* GameCalloc(const char* func, int count, size_t size){
  size_t total_size = count * size;
  size_t total = total_size + sizeof(size_t);

  size_t* raw = calloc(1, total);  // zero-initialized
  if (!raw) return NULL;

  *raw = total_size;
  TOTAL_SIZE += total_size;

  double mb = TOTAL_SIZE / (1024.0 * 1024.0);
  double gb = TOTAL_SIZE / (1024.0 * 1024.0 * 1024.0);
  //TraceLog(LOG_INFO,"%s is Allocating %zu bytes. Total %.2f MB (%.2f GB)\nat frame %i", func, total_size, mb, gb, WorldGetTime());

  return (void*)(raw + 1);
}

void* GameMalloc(const char* func, size_t size){
  size_t total = size + sizeof(size_t);
  size_t* raw = malloc(total);
  if (!raw) return NULL;

  *raw = size;               // store user size
  TOTAL_SIZE += size;

  double mb = TOTAL_SIZE / (1024.0 * 1024.0);
  double gb = TOTAL_SIZE / (1024.0 * 1024.0 * 1024.0);
//  TraceLog(LOG_INFO, "%s is Allocating %zu bytes. Total %.2f MB (%.2f GB)\n", func, size, mb, gb);

  return (void*)(raw + 1);   // return pointer after header
}

void GameFree(const char* func, void* ptr){
  if (!ptr)
    return;

  size_t* raw = ((size_t*)ptr) - 1;
  size_t size = *raw;
  if(size == 0)
    return;

  TOTAL_SIZE-= size;
  double mb = TOTAL_SIZE / (1024 * 1024);
  double gb = TOTAL_SIZE / (1024 * 1024 * 1024);
  //TraceLog(LOG_INFO, "%s is freeing %zu\n TOTAL SIZE %.2f MB (%.2f GB)\n", func, size, mb, gb);

  free(raw);

}

void* GameRealloc(const char* func, void* ptr, size_t new_size)
{
  if (!ptr)
    return GameMalloc(func, new_size);

  // Move back to header
  size_t* raw = ((size_t*)ptr) - 1;
  size_t old_size = *raw;

  // Allocate new memory with header
  size_t* new_raw = realloc(raw, new_size + sizeof(size_t));
  if (!new_raw) return NULL;

  *new_raw = new_size;

  TOTAL_SIZE += new_size;
  TOTAL_SIZE -= old_size;

  double mb = TOTAL_SIZE / (1024.0*1024.0);
  double gb = TOTAL_SIZE / (1024.0*1024.0*1024.0);
//      TraceLog(LOG_INFO,"%s realloc: %zu bytes (old %zu). Total %.2f MB (%.2f GB)\n",
  //        func, new_size, old_size, mb, gb);

  return (void*)(new_raw + 1);
};

