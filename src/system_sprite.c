#include "system_define.h"
#include "tool_lookup.h"

sprite_layer_t SPRITE_LAYERS[LAYER_DONE];

int SpriteDepthSort(const void* a, const void* b)
{
    Entity ea = EntityGet(&world.manager, *(int*)a);
    Entity eb = EntityGet(&world.manager, *(int*)b);
    
    position_t* pa = GET_COMPONENT(&world, ea, position_t, POS_ID);
    position_t* pb = GET_COMPONENT(&world, eb, position_t, POS_ID);
    
    return (int)(pa->pos.y - pb->pos.y);   // Higher Y drawn later
}

void SpriteRender(world_t* w, RenderLayer l)
{
  if (l < 0 || l >= LAYER_DONE) return;

  sprite_layer_t* layer = &SPRITE_LAYERS[l];   // ← Use pointer, no copy

  for (int j = 0; j < layer->count; j++)
  {
    int ent_id = layer->ents[j];
    Entity e = EntityGet(&w->manager, ent_id);
    sprite_t* s = GET_COMPONENT(w, e, sprite_t, SPR_ID);
    position_t* pos = GET_COMPONENT(w, e, position_t, POS_ID);

    if (s && pos)
      DrawSprite(s, pos->pos);
  }
}

void SpriteOnRender(void* o_data, void* s, payload_t* p) {
  render_ctx_t* r = p->data;
  world_t* w = o_data;

  SpriteRender(w, r->layer);
}

void SpriteDrawPrep(world_t* w){
  //TODO MARK SPRITE LAYER as dirty using component pool has_update on POSITION
  for(int i = LAYER_FLOOR; i < LAYER_UI; i++)
    qsort(SPRITE_LAYERS[i].ents, SPRITE_LAYERS[i].count, sizeof(int), SpriteDepthSort);
}

void SpriteLoad(world_t* w, Entity e)
{
  sprite_t* s = GET_COMPONENT(w, e, sprite_t, SPR_ID);
  if (!s) return;

  if (s->layer < 0 || s->layer >= LAYER_DONE) return;

  sprite_layer_t* layer = &SPRITE_LAYERS[s->layer];

  if (layer->ents == NULL) {
    TraceLog(LOG_ERROR, "CRITICAL: Layer %d ents pointer is NULL!", s->layer);
    return;
  }

  if (layer->count >= layer->cap) {
    TraceLog(LOG_WARNING, "Layer %d full", s->layer);
    return;
  }

  layer->ents[layer->count++] = e.id;
}

void SpritesInit(world_t* w){
  for(int i = 0; i < LAYER_DONE; i++){
    SPRITE_LAYERS[i].cap = MAX_LAYER_SPRITES;
    SPRITE_LAYERS[i].ents = GameCalloc("SpritesInit",
        MAX_LAYER_SPRITES, sizeof(int));

    SubjectAddObserver(LookupLayer(i), "SPRITES", SpriteOnRender, w);
  }
}
