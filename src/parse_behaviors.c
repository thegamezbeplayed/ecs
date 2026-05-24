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

    def->type = BT_LEAF;
    def->fn = StringToLeafFunc(fname);
    RegisterBehaviorDef(def);
  }

  cJSON* branches = cJSON_GetObjectItem(j, "branches");

  cJSON* tree;
  cJSON_ArrayForEach(tree, branches){
    behavior_define_t* branch = GameCalloc("ParseBehaviorDefs", 1, sizeof(behavior_define_t));
    branch->type = BT_SEQUENCE;
    Json_GetString(tree, "name", branch->name);

    char sname[MAX_NAME_LEN];
    Json_GetString(tree, "state", sname);
    branch->state = StringToState(sname);
    char tname[MAX_NAME_LEN];
    Json_GetString(tree, "type", tname);
    branch->type = StringToBehaviorType(tname);

    branch->param_overide = Json_GetBool(tree, "override");
    cJSON* kids = cJSON_GetObjectItem(tree, "children");

    cJSON* kid;
    cJSON_ArrayForEach(kid, kids){
      char cname[MAX_NAME_LEN];
      strcpy(cname, kid->valuestring);\
      branch->children[branch->num_children++] = hash_str_64(cname);
    };
      
    RegisterBehaviorDef(branch);
  }

  cJSON* roots = cJSON_GetObjectItem(j, "roots");
  cJSON* bn;
  cJSON_ArrayForEach(bn, roots){
    behavior_define_t* node = GameCalloc("ParseBehaviorDefs", 1, sizeof(behavior_define_t));

    node->type = BT_SEQUENCE;
    Json_GetString(bn, "name", node->name);
    node->is_root = true;    

    char sname[MAX_NAME_LEN];
    Json_GetString(bn, "state", sname);
    node->state = StringToState(sname);
    char tname[MAX_NAME_LEN];
    Json_GetString(bn, "type", tname);
    node->type = StringToBehaviorType(tname);

    cJSON* kids = cJSON_GetObjectItem(bn, "children");

    cJSON* kid;
    cJSON_ArrayForEach(kid, kids){
      char cname[MAX_NAME_LEN];
      strcpy(cname, kid->valuestring);\
      node->children[node->num_children++] = hash_str_64(cname);
    };
      
    RegisterBehaviorDef(node);
  }
}
