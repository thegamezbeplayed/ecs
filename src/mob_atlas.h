#define MOB_IMAGE_PATH      "mob_atlas.png"

#define NUM_MOB 45
static sub_texture_t MOB_SPRITES[NUM_MOB] = {
    { "slime_die_0_0",    "slime", 16, 16, 0, 0, 32, 32, 
      {1,26,18,8,8}, 8},
    { "slime_die_0_1",    "slime", 16, 16, 32, 0, 32, 32, .dur=8},
    { "slime_die_0_2",    "slime", 16, 16, 64, 0, 32, 32, .dur=8},
    { "slime_die_0_3",    "slime", 16, 16, 96, 0, 32, 32, .dur=8},
    { "slime_die_0_4",    "slime", 16, 16, 128, 0, 32, 32, .dur=8},

    { "slime_idle_0_0",   "slime", 16, 16, 160, 0, 32, 32, .dur=8},
    { "slime_idle_0_1",   "slime", 16, 16, 192, 0, 32, 32, .dur=8},
    { "slime_idle_0_2",   "slime", 16, 16, 224, 0, 32, 32, .dur=8},
    { "slime_idle_0_3",   "slime", 16, 16, 0, 32, 32, 32, .dur=8},

    { "slime_idle_180_0",   "slime", 16, 16, 160, 0, 32, 32, .dur=8, true, true},
    { "slime_idle_180_1",   "slime", 16, 16, 192, 0, 32, 32, .dur=8, true},
    { "slime_idle_180_2",   "slime", 16, 16, 224, 0, 32, 32, .dur=8, true},
    { "slime_idle_180_3",   "slime", 16, 16, 0, 32, 32, 32, .dur=8, true},

    { "slime_idle_90_0",  "slime", 16, 16, 32, 32, 32, 32, .dur=8},
    { "slime_idle_90_1",  "slime", 16, 16, 64, 32, 32, 32, .dur=8},
    { "slime_idle_90_2",  "slime", 16, 16, 96, 32, 32, 32, .dur=8},
    { "slime_idle_90_3",  "slime", 16, 16, 128, 32, 32, 32, .dur=8},

    { "slime_idle_270_0", "slime", 16, 16, 160, 32, 32, 32, .dur=8},
    { "slime_idle_270_1", "slime", 16, 16, 192, 32, 32, 32, .dur=8},
    { "slime_idle_270_2", "slime", 16, 16, 224, 32, 32, 32, .dur=8},
    { "slime_idle_270_3", "slime", 16, 16, 0, 64, 32, 32, .dur=8},

    { "slime_walk_0_0",   "slime", 16, 16, 32, 64, 32, 32, .dur=8},
    { "slime_walk_0_1",   "slime", 16, 16, 64, 64, 32, 32, .dur=8},
    { "slime_walk_0_2",   "slime", 16, 16, 96, 64, 32, 32, .dur=8},
    { "slime_walk_0_3",   "slime", 16, 16, 128, 64, 32, 32, .dur=8},
    { "slime_walk_0_4",   "slime", 16, 16, 160, 64, 32, 32, .dur=8},
    { "slime_walk_0_5",   "slime", 16, 16, 192, 64, 32, 32, .dur=8},


    { "slime_walk_180_0",   "slime", 16, 16, 32, 64, 32, 32, .dur=8, true},
    { "slime_walk_180_1",   "slime", 16, 16, 64, 64, 32, 32, .dur=8, true},
    { "slime_walk_180_2",   "slime", 16, 16, 96, 64, 32, 32, .dur=8, true},
    { "slime_walk_180_3",   "slime", 16, 16, 128, 64, 32, 32, .dur=8, true},
    { "slime_walk_180_4",   "slime", 16, 16, 160, 64, 32, 32, .dur=8, true},
    { "slime_walk_180_5",   "slime", 16, 16, 192, 64, 32, 32, .dur=8, true},

    { "slime_walk_90_0",  "slime", 16, 16, 224, 64, 32, 32, .dur=8},
    { "slime_walk_90_1",  "slime", 16, 16, 0, 96, 32, 32, .dur=8},
    { "slime_walk_90_2",  "slime", 16, 16, 32, 96, 32, 32, .dur=8},
    { "slime_walk_90_3",  "slime", 16, 16, 64, 96, 32, 32, .dur=8},
    { "slime_walk_90_4",  "slime", 16, 16, 96, 96, 32, 32, .dur=8},
    { "slime_walk_90_5",  "slime", 16, 16, 128, 96, 32, 32, .dur=8},

    { "slime_walk_270_0", "slime", 16, 16, 160, 96, 32, 32, .dur=8},
    { "slime_walk_270_1", "slime", 16, 16, 192, 96, 32, 32, .dur=8},
    { "slime_walk_270_2", "slime", 16, 16, 224, 96, 32, 32, .dur=8},
    { "slime_walk_270_3", "slime", 16, 16, 0, 128, 32, 32, .dur=8},
    { "slime_walk_270_4", "slime", 16, 16, 32, 128, 32, 32, .dur=8},
    { "slime_walk_270_5", "slime", 16, 16, 64, 128, 32, 32, .dur=8},
};

