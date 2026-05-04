#define TILE_IMAGE_PATH      "tile_atlas.png"
#define NUM_TILE    10

static sub_texture_t TILE_SPRITES[NUM_TILE] = {
  { "tile_dirt",          "dirt",        8, 8, 0, 0, 16, 16},
  { "tile_grass",         "grass",       8, 8, 16, 0, 16, 16},
  { "tile_dirt_patch_0",  "dirt_patch",  8, 8, 32, 0, 16, 16},
  { "tile_dirt_patch_1",  "dirt_patch",  8, 8, 48, 0, 16, 16},
  { "tile_dirt_patch_2",  "dirt_patch",  8, 8, 64, 0, 16, 16},
  { "tile_dirt_patch_3",  "dirt_patch",  8, 8, 80, 0, 16, 16},
  { "tile_grass_decor_1", "grass_decor", 8, 8, 96, 0, 16, 16},
  { "tile_grass_decor_2", "grass_decor", 8, 8, 112, 0, 16, 16},
  { "tile_grass_decor_3", "grass_decor", 8, 8, 0, 16, 16, 16},
  { "tile_grass_decor_4", "grass_decor", 8, 8, 16, 16, 16, 16},
};
