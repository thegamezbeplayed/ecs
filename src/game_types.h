#ifndef __GAME_TYPES__
#define __GAME_TYPES__
#include "game_tools.h" 

#define ParamRead(o, T) ((T*)((o)->data))

typedef struct{
  Cell    pos, last_pos, facing;
  Vector2 vpos, last_vpos, dir;
}position_t;
position_t* InitPosition(Cell pos);
void PositionAddStep(position_t*, Vector2);

static inline bool LESS_THAN(int a, int b){
  return a<b;
}

static inline bool GREATER_THAN(int a, int b){
  return a>b;
}

static inline bool NOT_EQUAL_TO(int a, int b){
  return a!=b;
}


static inline bool EQUAL_TO(int a, int b){
  return a==b;
}

static inline bool ALWAYS(int a, int b){
  return true;
}

static inline bool NEVER(int a, int b){
  return false;
}

typedef enum {
  DATA_NONE,
  DATA_ARRAY,
  DATA_PARAM,
  DATA_INT,
  DATA_FLOAT,
  DATA_BOOL,
  DATA_PTR,
  DATA_CELL,
  DATA_UINT64,
  DATA_GOUID,
  DATA_STRING,
  DATA_ENT,
  DATA_EVENT,
  DATA_MAP_CELL,
  DATA_ELEM,
  DATA_LEVEL,
  DATA_CD,
  DATA_SPRITE,
  DATA_SLICE,
  DATA_ALL,
} DataType;

typedef struct{
  uint64_t          uid;
  DataType          type;
  void*             data;
  size_t            size;
}param_t;

typedef struct{
  int     count;
  param_t *params;
}payload_t;
payload_t* InitPayload(int count, param_t[count]);
payload_t* InitPayloadSingle(param_t);

static param_t EMPTY_PARAM = {0,DATA_NONE};
static param_t ParamCopyObj(DataType type, uint64_t uid, const void* src, size_t size){
  param_t o;
  o.type = type;
  o.size = size;
  o.uid = uid;

  if (src && size > 0)
  {
    o.data = GameMalloc("ParamCopyObj", size);
    memcpy(o.data, src, size);
  }
  else
  {
    o.data = NULL;
  }

  return o;
}

static param_t ParamMakeObj(DataType type, uint64_t uid, void* src) {
  param_t o;
  o.type = type;
  o.size = 0;
  o.uid = uid;
  o.data = src;
  return o;
}

static param_t ParamMakeArray(DataType type, uint64_t uid, void* src, int count, size_t size){

  param_t p = {0};
  p.uid = uid;
  p.type = type;
  p.size  =  count;
  p.data = src;
  return p;

  if( src && count > 0)
    p.data = GameMalloc("ParamMakeArray", size * count);
    memcpy(p.data, src, size * count);

  return p;

}

static param_t ParamMake(DataType type, size_t size, const void* src) {
  param_t o;
  o.type = type;
  o.size = size;

  o.uid = -1;
  o.data = GameMalloc("ParamMake", o.size);
  memcpy(o.data, src, o.size);
  return o;
}

static param_t ParamMakeString(const char* s) {
  size_t len = strlen(s)+1;
  return ParamMake(DATA_STRING, len, s);
}

static inline const char* ParamReadString(param_t* o){
  assert(o);
  assert(o->type == DATA_STRING);
  assert(o->data);
  assert(o->size > 0);

  return (const char*)o->data;
}

static inline float ParamReadFloat(const param_t* o) {
    assert(o->type == DATA_FLOAT);
    assert(o->size == sizeof(float));
    return *(float*)o->data;
} 
  
static inline int ParamReadInt(const param_t* o) {
    assert(o->type == DATA_INT);
    assert(o->size == sizeof(int));
    return *(int*)o->data;
}
  
static inline Cell ParamReadCell(const param_t* o){
  assert(o->type == DATA_CELL);
  return *(Cell*)o->data;

} 

typedef bool (*ParamCompareFn)(param_t *a, param_t *b);
static bool ParamCompareGreater(param_t *a, param_t *b){
  int aval = 0, bval = 0;
  int adec = 0, bdec = 0;

  switch(a->type){
    case DATA_FLOAT:
      Cell afval = float_to_ints(*ParamRead(a, float));
      Cell bfval = float_to_ints(*ParamRead(b, float));
      aval = afval.x;
      adec = afval.y;

      bval = bfval.x;
      bdec = bfval.y;
      aval = aval*100 + adec;
      bval = bval*100 + bdec;
      break;
    case DATA_INT:
      aval = *ParamRead(a, int);
      bval = *ParamRead(b, int);
      break;
    default:
      return false;
      break;
  }
  return GREATER_THAN(aval, bval);
}

static bool ParamCompareAnd(param_t *a, param_t *b){
  uint64_t abits = *ParamRead(a, uint64_t);
  uint64_t bbits = *ParamRead(b, uint64_t);

  return ((abits & bbits) > 0);
}

static bool ParamCompareLesser(param_t *a, param_t *b){
  return LESS_THAN(*ParamRead(a, int) ,*ParamRead(b, int));
}

static bool ParamCompare(param_t *a, param_t *b, ParamCompareFn fn){
  assert(a->type == b->type);

  return fn(a ,b);

}

#endif
