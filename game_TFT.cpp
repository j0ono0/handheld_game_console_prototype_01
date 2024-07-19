#include "game_TFT.h"


void Extended_Tft::drawIntro()
{   
    fillScreen(ILI9341_YELLOW);
    setTextColor(ILI9341_BLACK);
    setCursor(50, 50);
    setTextSize(3);
    println("Sokoban!");
    setCursor(50, 80);
    setTextSize(2);
    println("Press start");
}

void Extended_Tft::drawMapComplete()
{
    fillScreen(ILI9341_RED);
    setTextColor(ILI9341_WHITE);
    setCursor(20, 50);
    setTextSize(3);
    println("Map complete");
    setCursor(10, 70);
    setTextSize(2);
    println("Press start to continue.");
}

void Extended_Tft::drawSuccess()
{
    fillScreen(ILI9341_RED);
    setTextColor(ILI9341_WHITE);
    setCursor(20, 50);
    setTextSize(2);
    println("You win Sokoban!");
    setCursor(10, 70);
    setTextSize(2);
    println("Press start to play again.");
}

void Extended_Tft::drawCellBuffer(uint16_t *buf, int x, int y)
{
    writeRect(x * TERRAIN_UNIT, y * TERRAIN_UNIT, TERRAIN_UNIT, TERRAIN_UNIT, buf);
}
