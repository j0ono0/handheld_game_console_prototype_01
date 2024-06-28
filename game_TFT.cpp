#include "game_TFT.h"


void Extended_Tft::drawSprite(int x, int y, uint16_t color){

	beginSPITransaction(_clock);
	setAddr(x * GRID_SIZE, y * GRID_SIZE, x * GRID_SIZE + GRID_SIZE - 1, y * GRID_SIZE + GRID_SIZE - 1);
	writecommand_cont(ILI9341_RAMWR);
    
    for(int x = 0; x < GRID_SIZE; x++){
        for(int y = 0; y < GRID_SIZE; y++){
            writedata16_cont(color);
        }
    }	
    writedata16_last(color);

	endSPITransaction();
}
void Extended_Tft::eraseSprite(int x, int y, int width, int height){
    
	beginSPITransaction(_clock);
	setAddr(x * GRID_SIZE + (GRID_SIZE - width), y * GRID_SIZE + (GRID_SIZE - height), x * GRID_SIZE + width - 1, y * GRID_SIZE + height - 1);
	writecommand_cont(ILI9341_RAMWR);
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            writedata16_cont(COLOR_FLOOR);
        }
    }	
    writedata16_last(COLOR_FLOOR);

	endSPITransaction();
}

void Extended_Tft::drawSprites(struct Entity array[], int len)
{
    for(int i = 0; i < len; i++){
        drawSprite(array[i].x, array[i].y, array[i].color);
    }
}

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

void Extended_Tft::drawMapComplete(int mapNum)
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

void Extended_Tft::drawSprite(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors)
{

	for(int sy=0; sy < h; sy++) 
    {
		for(int sx=0; sx < w; sx++) 
        {
            uint16_t pxColor = *pcolors++;
            if(pxColor != COLOR_TRANSPARENT){
                drawPixel(x * GRID_SIZE + sx + (GRID_SIZE - w), y * GRID_SIZE + sy + (GRID_SIZE - h), pxColor);
            }
        }
	}
}