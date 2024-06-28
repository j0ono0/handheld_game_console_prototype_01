
#ifndef game_TFTClass_h
#define game_TFTClass_h

#include "ILI9341_t3.h"
#include "SPI.h"
#include "font_Arial.h"
#include "engine.h"

// # define GRID_WIDTH 8
// # define GRID_HEIGHT 6
// # define GRID_SIZE 40

# define GRID_WIDTH 20
# define GRID_HEIGHT 15
# define GRID_SIZE 16


#define COLOR_FLOOR 0xbe5c
#define COLOR_FLOOR_TARGET 0x93d8
#define COLOR_BOX 0xc4c7
#define COLOR_BOX_TARGET 0xfb42
#define COLOR_WALL 0x8494
#define COLOR_PLAYER 0x3c04

#define COLOR_TRANSPARENT 0x07e0

class Extended_Tft : public ILI9341_t3  {
public:
    Extended_Tft(uint8_t cs, uint8_t dc) : ILI9341_t3{cs, dc}{}

    void drawSprite(int x, int y, uint16_t color);
    void eraseSprite(int x, int y, int width, int height);
    void drawSprites(struct Entity array[], int len);

    void drawIntro();
    void drawMapComplete(int mapNum);
    void drawSuccess();
    void drawSprite(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors);

};

#endif