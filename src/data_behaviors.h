#ifndef __DAT_BEHAVE__
#define __DAT_BEHAVE__

#define BN_COUNT 10

static behavior_d BEHAVE[BN_COUNT] = {
  {"CHANGE_STATE",0, BT_LEAF, LeafChangeState},
  {"CHECK_AGGRO", 0, BT_LEAF, LeafCheckAggro},
  {"ACQ_DEST",    0, BT_LEAF, LeafAcquireDestination},
  {"MOVE_DEST",   0, BT_LEAF, LeafMoveToDestination},  
  {"GET_AGGRO",   0, BT_SEQUENCE, NULL, 2, 
    {"CHECK_AGGRO", "CHANGE_STATE"}, true, STATE_AGGRO},
  {"GET_DEST",    0, BT_SEQUENCE, NULL, 2, {"ACQ_DEST", "MOVE_DEST"}},
  {"WANDER",      STATE_IDLE, BT_SELECTOR, NULL, 2, {"GET_AGGRO", "GET_DEST"}},
  {"ATTACK",      STATE_AGGRO, BT_SEQUENCE, NULL, 1, {"CHANGE_STATE"}}
};


#endif
