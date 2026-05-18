#include "game_systems.h"

sprite_layer_t SPRITE_LAYERS[LAYER_DONE] = {0};

void SpriteRender(world_t* w, RenderLayer l)
{
  if (l < 0 || l >= LAYER_DONE) return;

  sprite_layer_t* layer = &SPRITE_LAYERS[l];   // ← Use pointer, no copy

  for (int j = 0; j < layer->count; j++)
  {
    Entity e = EntityGet(&w->manager, layer->ents[j]);
    sprite_t* s   = GET_COMPONENT(w, e, sprite_t, SPR_ID);
    position_t* pos = GET_COMPONENT(w, e, position_t, POS_ID);

    if (s && pos)
      DrawSprite(s, pos->vpos);
  }
}

void SpriteOnRender(void* o_data, void* s, void* e_data) {
  render_ctx_t* r = e_data;
  world_t* w = o_data;

  SpriteRender(w, r->layer);
}

void SpriteLoad(world_t* w, Entity e)
{
  sprite_t* s = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  if (!s){
    TraceLog(LOG_WARNING, " ==== SPRITE LOAD ===\n sprite not found for Ent: %i", e.id);
    return;
  }

  if (s->layer < LAYER_BG || s->layer >= LAYER_DONE)
  {
    TraceLog(LOG_WARNING, "==== SPRITE LOAD ===\n Ent %i invalid sprite layer %i", e.id, s->layer);
    return;
  }

  sprite_layer_t* layer = &SPRITE_LAYERS[s->layer];

  if (layer->count >= layer->cap)
  {
    TraceLog(LOG_WARNING, "Sprite layer %d is full (cap=%d)", s->layer, layer->cap);
    return;
  }

  // Safe assignment
  layer->ents[layer->count] = e.id;
  layer->count++;
}

void SpritesInit(world_t* w){
  for(int i = 0; i < LAYER_DONE; i++){
    SPRITE_LAYERS[i].cap = MAX_LAYER_SPRITES;
    SPRITE_LAYERS[i].ents = GameCalloc("SpritesInit",
        MAX_LAYER_SPRITES, sizeof(int));
  }
  
  SubjectAddObserver(&renderer, SpriteOnRender, w);
}
