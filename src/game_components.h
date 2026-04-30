#ifndef __GAME_COMP__
#define __GAME_COMP__

#include "screens.h"
#include "game_physics.h"
#include "game_behaviors.h"
#include "game_assets.h"
#include "game_control.h"

#define MAX 1024
#define MAX_COMPONENTS 512
#define MAX_PLAYERS 2

#define FIRST(pool) \
    ((pool).size == 0 ? -1 : (pool).entities[0])
#define COMP_MASK 0xFFFFFFFF

typedef uint64_t comp_id_t;

static inline comp_id_t make_comp(uint32_t c) {
    return (comp_id_t)c;
}

static inline comp_id_t make_pair(uint32_t r, uint32_t o) {
    return ((uint64_t)r << 32) | o;
}

static inline uint32_t pair_rel(comp_id_t id) {
    return (uint32_t)(id >> 32);
}

static inline uint32_t pair_obj(comp_id_t id) {
    return (uint32_t)(id & COMP_MASK);
}

static inline int is_pair(comp_id_t id) {
    return (id >> 32) != 0;
}
#endif
