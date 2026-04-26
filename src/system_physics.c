#include "game_register.h"
#include "game_systems.h"

void PhysicsCollision(system_pool_t* s, component_registry_t* c){

}

void InitPhysicsSystem(phys_st* p){

  RegisterScheduleStep(UPDATE_PRE, PhysicsCollision);

}
