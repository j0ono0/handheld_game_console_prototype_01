#ifndef game_config_h
#define game_config_h


// SPI hardware pins
#define TFT_DC 9
#define TFT_CS 10

/*
    Note: 
    Terrain grid size is chosen for more effecient graphics.
    Env grid at x2 terrain size makes for better UI. 
    
    ?? Generally everything should be expressed in
    env-grid leaving terrain-grid for lower level functions. ??
*/

# define SCREEN_WIDTH 320
# define SCREEN_HEIGHT 240

// Terrain tiles are split into a grid. 
// Each cell of this 'terrain grid' is 8x8 pixels
# define TERRAIN_WIDTH 40
# define TERRAIN_HEIGHT 30
# define TERRAIN_UNIT 8

// Entities are tracked within the an 'environment grid'. 
// These grid cells are 16x16 pixels. 
# define ENV_WIDTH 20
# define ENV_HEIGHT 15
# define ENV_UNIT 16


#define COLOR_FLOOR 0xbe5c
#define COLOR_FLOOR_TARGET 0x9d9b
#define COLOR_BOX 0xc4c7
#define COLOR_BOX_ACTIVE 0xfb42
#define COLOR_WALL 0x8494
#define COLOR_PLAYER 0x3c04
#define COLOR_WATER 0x25b9

#define COLOR_TRANSPARENT 0x07e0

#define MAX_ENTITIES 100

#endif