#include "game_TFT.h"


#include "sprite_crate.c"
#include "sprite_crate_active.c"
#include "sprite_crate_special.c"
#include "sprite_floor.c"
#include "sprite_platform.c"
#include "sprite_plr.c"
#include "sprite_target.c"
#include "sprite_wall_n.c"
#include "sprite_wall_e.c"
#include "sprite_wall_s.c"
#include "sprite_wall_w.c"
#include "sprite_wall_low.c"


#include "game_maps.c"
extern const char maps_20x15[2][15][21];

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

void Extended_Tft::drawSprite(int x, int y, int w, int h, uint16_t *pcolors)
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

void Extended_Tft::drawSpriteCell(int x, int y, uint16_t *sprite, int offset)
{
    uint16_t *pixels = sprite + offset;
    // Draw a GRID_SIZE cell from sprite data 
    for(int sy=0; sy < GRID_SIZE; sy++) 
    {
		for(int sx=0; sx < GRID_SIZE; sx++) 
        {
            uint16_t color = *pixels++;
            if(color != COLOR_TRANSPARENT){
                drawPixel(x * GRID_SIZE + sx, y * GRID_SIZE + sy, color);
            }
        }
	}
}

void Extended_Tft::drawPlr(int x, int y)
{
    // Draw top half in cell above entity location
    drawSpriteCell(x, y-1, (uint16_t *)sprite_plr, 0);
    // Draw bottom half at location
    drawSpriteCell(x, y, (uint16_t *)sprite_plr, 255);
}

void Extended_Tft::drawCrate(int x, int y, bool state)
{
    if(state == true){
        drawSpriteCell(x, y, (uint16_t *)sprite_crate_active, 0);
        return;
    }
    drawSpriteCell(x, y, (uint16_t *)sprite_crate, 0);
}

void Extended_Tft::drawTarget(int x, int y)
{
        drawSpriteCell(x, y, (uint16_t *)sprite_target, 0);
}

void Extended_Tft::drawFloor(int x, int y)
{
        drawSpriteCell(x, y, (uint16_t *)sprite_floor, 0);
}

void Extended_Tft::drawWallLow(int x, int y)
{
        drawSpriteCell(x, y-1, (uint16_t *)sprite_wall_low, 0);
        drawSpriteCell(x, y, (uint16_t *)sprite_wall_low, 256);
}

void Extended_Tft::drawTerrain(int mapIndex, int x, int y, bool overlap)
{
    switch(maps_20x15[mapIndex][y][x])
    {
        case '#':
        case 'N':
            drawSpriteCell(x, y, (uint16_t *)sprite_wall_low, 256);
            if(overlap)
                drawSpriteCell(x, y-1, (uint16_t *)sprite_wall_low, 0);
            return;
        case 'X':
        case 'B':
            drawSpriteCell(x, y, (uint16_t *)sprite_target, 0);
            return;
        default:
            drawSpriteCell(x, y, (uint16_t *)sprite_floor, 0);
            return;
    }
}

void Extended_Tft::drawTerrainOverlap(int mapIndex, int x, int y)
{
    // Out of map boundaries
    if(y >= GRID_HEIGHT)
        return;
    // Draw the overlapping section of the sprite in front into this location
    switch(maps_20x15[mapIndex][y+1][x])
    {
        case '#':
        case 'N':
            drawSpriteCell(x, y, (uint16_t *)sprite_wall_low, 0);
            return;
        case 'X':
        case 'B':
        case '.':
            // Sprite does not overlap
            return;
        default:
            break;
    }
}