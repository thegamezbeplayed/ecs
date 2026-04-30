#include "game_define.h"
#include "game_register.h"

uint64_t ANIM_ID;


void RegisterComponentData(world_t* world) {
    ANIM_ID = REGISTER_COMPONENT(world, sizeof(anim_player_t));
}
