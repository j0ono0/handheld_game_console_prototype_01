#include "game_TFT.h"

#include "title_screen_main.c"

void Extended_Tft::drawIntro()
{   

    writeRect(0, 0, 320, 240, title_screen_main);

}

void Extended_Tft::drawMapComplete()
{
    fillScreen(ILI9341_RED);
    setTextColor(ILI9341_WHITE);
    setTextSize(3);
    setCursor(20, 50);
    println("Section complete");
    setTextSize(1);
    setCursor(160, 200);
    println("Press start to continue.");


}


void Extended_Tft::drawSuccess()
{
    fillScreen(0x0327); //dk green
    setTextColor(ILI9341_WHITE);
    setTextSize(4);
    setCursor(20, 50);
    println("Success!");
    setCursor(160, 200);
    setTextSize(1);
    println("Press start to play again.");
}

void Extended_Tft::drawCellBuffer(uint16_t *buf, int x, int y)
{
    writeRect(x * TERRAIN_UNIT, y * TERRAIN_UNIT, TERRAIN_UNIT, TERRAIN_UNIT, buf);
}

void Extended_Tft::drawSplashScreen(const uint16_t *img)
{
    writeRect(0, 0, 320, 240, img);
}
