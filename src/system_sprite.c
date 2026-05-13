#include "game_assets.h"
#include "game_systems.h"

static sprite_layer_t SPRITE_LAYERS[LAYER_DONE] = {0};

void SpriteRender(world_t* w, RenderLayer l){
  sprite_layer_t sprl = SPRITE_LAYERS[l];

  for (int j = 0; j < sprl.count; j++){
    Entity e = EntityGet(&w->manager, sprl.ents[j]); 
    sprite_t* s      = GET_COMPONENT(w, e, sprite_t, SPR_ID);
    position_t* pos  = GET_COMPONENT(w, e, position_t, POS_ID);

    DrawSprite(s, pos->vpos);
  }
}

void SpriteViewEvent(event_t* ev, void* data){
  world_t* w = data;

  SpriteRender(w, ev->eid);
}

void SpriteLoad(world_t* w, Entity e){
  sprite_t* s      = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  
  if(s->layer < LAYER_BG || s->layer >= LAYER_DONE){
    TraceLog(LOG_WARNING, "==== SPRITE LOAD ===\n Ent %i invalid sprite layer %i", e.id, s->layer);
    return;
  }

  int *slot = &SPRITE_LAYERS[s->layer].ents[SPRITE_LAYERS[s->layer].count++];

  *slot = e.id;

}

void SpritesInit(world_t* w){
  notification n = ViewEvent_ToNotif(VIEW_EVENT_DRAW);

  for(int i = 0; i < LAYER_DONE; i++){
    SPRITE_LAYERS[i].cap = MAX_LAYER_SPRITES;
    SPRITE_LAYERS[i].ents = GameCalloc("SpritesInit",
        MAX_LAYER_SPRITES, sizeof(int));
    
    TargetSubscribe(n, SpriteViewEvent, w, i);
  }
}

