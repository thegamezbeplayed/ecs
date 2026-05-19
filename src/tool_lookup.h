#ifndef __TOOL_LOOK__
#define __TOOL_LOOK__
static const char* RENDER_LAYER_LOOK[LAYER_DONE] = {
  "LAYER_BACKGROUND",
  "LAYER_FLOOR",
  "LAYER_MAIN",
  "LAYER_TOP",
  "LAYER_UI",
};

static const char* LookupLayer(int l){
  return RENDER_LAYER_LOOK[l];

}
#endif

