#ifndef __DEB_DEF__
#define __DEB_DEF__


static Font dfont;// = LoadFontEx("resources/fonts/kenney-pixel-square.ttf", 64, 0, 0);
static int outlineThickness = 2;
static void InitDebug(void){
  dfont = LoadFontEx("resources/fonts/kenney-pixel-square.ttf", 64, 0, 0);
  SetTextureFilter(dfont.texture, TEXTURE_FILTER_POINT);
}

static void DrawDebugText(const char* text, float fontSize, Vector2 pos, Color outlineColor, Color textColor)
{
    if (!text || text[0] == '\0') return;

    // Draw outline (8 directions for good coverage)
    for (int dx = -outlineThickness; dx <= outlineThickness; dx++)
    {
        for (int dy = -outlineThickness; dy <= outlineThickness; dy++)
        {
            if (dx == 0 && dy == 0) continue;   // skip center

            DrawTextEx(dfont, text,
                       (Vector2){ pos.x + dx, pos.y + dy },
                       fontSize, 1, outlineColor);
        }
    }

    // Draw main text on top
    DrawTextEx(dfont, text, pos, fontSize, 1, textColor);
}


#endif
