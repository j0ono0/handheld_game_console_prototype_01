
#ifndef game_TFTClass_h
#define game_TFTClass_h

#include "ILI9341_t3.h"
#include "SPI.h"
#include "font_Arial.h"
#include "config.h"


class Extended_Tft : public ILI9341_t3  {
public:
    Extended_Tft(uint8_t cs, uint8_t dc) : ILI9341_t3{cs, dc}{}

    void drawSprite(int x, int y, uint16_t color);
    void eraseSprite(int x, int y, int width, int height);

    void drawIntro();
    void drawMapComplete(int mapNum);
    void drawSuccess();
    void drawSprite(int x, int y, int w, int h, uint16_t *pcolors);
    void drawSpriteCell(int x, int y, uint16_t *pxColors, int offset);
    void drawPlr(int x, int y);
    void drawCrate(int x, int y, bool state);
};

#endif