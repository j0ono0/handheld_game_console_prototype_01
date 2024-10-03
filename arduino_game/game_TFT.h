
#ifndef game_TFTClass_h
#define game_TFTClass_h

#include "ILI9341_t3.h"
#include <XPT2046_Touchscreen.h>
#include "SPI.h"
#include <SD.h>
#include "font_Arial.h"
#include "config.h"

class Extended_Tft : public ILI9341_t3  {
public:
    Extended_Tft(uint8_t cs, uint8_t dc) : ILI9341_t3{cs, dc}{}

    void drawIntro();
    void drawDemo1();
    void drawMapComplete();
    void drawSuccess();
    void drawCellBuffer(uint16_t *buf, int x, int y);
    void drawSplashScreen(const uint16_t *img);

    void showTouchData(TS_Point p);
    
};

#endif