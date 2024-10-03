#include "game_TFT.h"

extern const uint16_t title_screen_main[];
extern const uint16_t demo_screen_01[];

void Extended_Tft::drawIntro()
{   
    writeRect(0, 0, 320, 240, title_screen_main);
}

void Extended_Tft::drawDemo1()
{   
    writeRect(0, 0, 320, 240, demo_screen_01);
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


void Extended_Tft::showTouchData(TS_Point p)
{
    
        fillScreen(ILI9341_BLACK);
        setTextColor(ILI9341_YELLOW);
        setFont(Arial_60);
        setCursor(60, 80);
        print("Touch");

        fillRect(100, 150, 140, 60, ILI9341_BLACK);
        setTextColor(ILI9341_GREEN);
        setFont(Arial_24);
        setCursor(100, 150);
        print("X = ");
        print(p.x);
        setCursor(100, 180);
        print("Y = ");
        print(p.y);
        Serial.print(", x = ");
        Serial.print(p.x);
        Serial.print(", y = ");
        Serial.println(p.y);
        return;
}