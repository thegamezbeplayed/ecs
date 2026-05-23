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

    Json_GetString(leaf, "name", node->name);
    
    cJSON* kids = cJSON_GetObjectItem(bn, "children");

    cJSON* kid;
    cJSON_ArrayForEach(kid, kids){
      char cname[MAX_NAME_LEN];
      strcpy(cname, kid->valuestring);
      node->children[node->num_children++] = hash_str_64(cname);
      RegisterBehaviorDef(node);
    };
  }
}
