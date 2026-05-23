#include "util_tools.h"
#include "util_parse.h"
#include "behavior_define.h"
#include "tool_lookup.h"

bool ParseBehaviorDefs(cJSON* j){
  cJSON* leafs = cJSON_GetObjectItem(j, "leafs");

  cJSON* leaf;
  cJSON_ArrayForEach(leaf, leafs){
    behavior_define_t* def = GameCalloc("ParseBehaviorDefs", 1, sizeof(behavior_define_t));

    Json_GetString(leaf, "name", def->name);
    char fname[MAX_NAME_LEN];
    Json_GetString(leaf, "fn", fname);

    def->fn = StringToLeafFunc(fname);
    RegisterBehaviorDef(def);
  }

  cJSON* roots = cJSON_GetObjectItem(j, "roots");

  cJSON* bn;
  cJSON_ArrayForEach(bn, roots){

    behavior_define_t* node = GameCalloc("ParseBehaviorDefs", 1, sizeof(behavior_define_t));

  }
}
