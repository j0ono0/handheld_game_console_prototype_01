
#ifndef game_TFTClass_h
#define game_TFTClass_h

#include "ILI9341_t3.h"
#include "SPI.h"
#include "font_Arial.h"
#include "config.h"



class Extended_Tft : public ILI9341_t3  {
public:
    Extended_Tft(uint8_t cs, uint8_t dc) : ILI9341_t3{cs, dc}{}

    void drawIntro();
    void drawMapComplete(int mapNum);
    void drawSuccess();
    void drawCellBuffer(uint16_t *buf, int x, int y);
    
};

#endif