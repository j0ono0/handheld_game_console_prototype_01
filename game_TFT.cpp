#include "game_TFT.h"


void Extended_Tft::drawIntro()
{   
    fillScreen(ILI9341_YELLOW);
    setTextColor(ILI9341_BLACK);
    setTextSize(4);
    setCursor(20, 50);
    println("Journey");
    setTextSize(2);
    setCursor(20, 80);
    println("by arrangement");
    setTextSize(1);
    setCursor(160, 200);
    println("Press start");
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
