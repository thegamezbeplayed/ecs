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

    // Draw background
    Vector2 size = MeasureTextEx(dfont, text, fontSize, 1);
    Rectangle bg = {pos.x - 2, pos.y - 2, size.x + 4, size.y + 4};
    DrawRectangleRec(bg, Fade(BLACK, 0.55f));

    // Draw main text on top
    DrawTextEx(dfont, text, pos, fontSize, 1, textColor);
}


#endif
