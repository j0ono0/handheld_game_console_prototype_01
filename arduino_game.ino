/** NOTES **********************

display is 320 x 240

*******************************/

#include "game_TFT.h"
#include "input.h"
#include "engine.h"
#include "game_maps.h"


// SPI hardware pins
#define TFT_DC 9
#define TFT_CS 10

Extended_Tft tft = Extended_Tft(TFT_CS, TFT_DC);


//Store all entities in a repository
Entity entity_repo[MAX_ENTITIES];
int er_length;

Entity *plr1;

enum GameMode gameMode;
int currentMap;

uint16_t colorOfType(enum EntityType type)
{
    uint16_t color = COLOR_FLOOR;
    switch (type)
    {
    case plr_t:
        color = COLOR_PLAYER;
        break;
    case crate_t:
        color = COLOR_BOX;
        break;
    case crate_active_t:
        color = COLOR_BOX_ACTIVE;
        break;
    case goal_t:
        color = COLOR_FLOOR_TARGET;
        break;
    case wall_t:
        color = COLOR_WALL;
        break;
    case floor_t:
        color = COLOR_FLOOR;
        break;
    }
    return color;
}

void drawTerrain(int mapIndex, int x, int y)
{
    uint16_t color = COLOR_FLOOR;
    if (maps_20x15[currentMap][y][x] == '#')
    {
        color = COLOR_WALL;
    }
    else if (maps_20x15[currentMap][y][x] == 'B' || maps_20x15[currentMap][y][x] == 'X')
    {
        color = COLOR_FLOOR_TARGET;
    }
    tft.fillRect(x*GRID_SIZE, y*GRID_SIZE, GRID_SIZE, GRID_SIZE, color);
}

void drawEntity(Entity *entity)
{
    switch(entity->type)
    {
        case plr_t:
            tft.drawPlr(entity->x, entity->y);
            break;
        case crate_t:
            tft.drawCrate(entity->x, entity->y, false);
            break;
        case crate_active_t:
            tft.drawCrate(entity->x, entity->y, true);
            break;
        default:
             tft.fillRect(entity->x * GRID_SIZE, entity->y * GRID_SIZE, GRID_SIZE, GRID_SIZE, colorOfType(entity->type));
    };
}

void drawAssets(Entity *entity_repo, int index)
{
    // Draw all terrain
    for(int y=0; y < GRID_HEIGHT; y++)
    {
        for(int x=0; x < GRID_WIDTH; x++)
        {
            drawTerrain(currentMap, x, y);
        }
    }
    // Draw all entities
    for(int i = 0; i <= index; ++i)
    {
        drawEntity(&entity_repo[i]);
    }
}


void buildAssets(const char gameMap[GRID_HEIGHT][GRID_WIDTH + 1])
{
    // Output map to console
    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            if (gameMap[row][col] == "."[0])
            {
                Serial.print(" ");
            }
            else
            {
                Serial.print(gameMap[row][col]);
            }
        }
        Serial.print('\n');
    }

    // Populate entityStore
    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            if (gameMap[row][col] == "B"[0])
            {
                createEntity(entity_repo, &er_length, crate_active_t, col, row);
            }
            else if (gameMap[row][col] == "b"[0])
            {
                createEntity(entity_repo, &er_length, crate_t, col, row);
            }
            else if (gameMap[row][col] == "@"[0])
            {
                // Assign entity as plr1 for easy reference
                plr1 = createEntity(entity_repo, &er_length, plr_t, col, row);
            }
        }
    }
    Serial.print("entityRepo populated, count: ");
    Serial.println(er_length);
    Serial.println("free memory: ");
    Serial.println(availableMemory());
}

void setup()
{
    Serial.begin(9600);

    setupButtonInputs();

    er_length = -1;
    gameMode = intro;

    currentMap = 0;

    buildAssets(maps_20x15[currentMap]);
    
    tft.begin();
    tft.setRotation(1);
    tft.drawIntro();

}

void loop()
{
    int userInput = readUserInput();

    if (gameMode != inGame)
    {
        if (userInput == 7)
        {
            Serial.println("starting game mode.");
            gameMode = inGame;
            tft.fillScreen(COLOR_FLOOR);
            drawAssets(entity_repo, er_length);
            return;
        }
        else
        {
            // Wait for start
            return;
        }
    }

    // Progress game
    if (userInput >= 3 && userInput <= 6)
    {
        int nextX, nextY;
        int dx = 0;
        int dy = 0;
        switch (userInput)
        {
        case BTN_N:
            dy = -1;
            break;
        case BTN_S:
            dy = 1;
            break;
        case BTN_W:
            dx = -1;
            break;
        case BTN_E:
            dx = 1;
            break;
        }
        nextX = plr1->x + dx;
        nextY = plr1->y + dy;

        if (!inbounds(nextX, nextY))
        {
            Serial.println("Out of bounds!");
            return;
        }
        else if (terrainBlocksMovement(currentMap, nextX, nextY))
        {
            Serial.println("There is no way through here.");
            return;
        }
        struct Entity *crate = entityAtLocation(entity_repo, er_length, nextX, nextY);
        if (crate != NULL)
        {
            if (
                !inbounds(crate->x + dx, crate->y + dy) || terrainBlocksMovement(currentMap, crate->x + dx, crate->y + dy) || entityAtLocation(entity_repo, er_length, crate->x + dx, crate->y + dy) != NULL )
            {
                Serial.println("This crate isn't budging!");
                return;
            }
            else
            {
                Serial.println("moving crate.");
                drawTerrain(currentMap, crate->x, crate->y);
                // Update crate location
                crate->x += dx;
                crate->y += dy;
                // Update crate status
                updateCrate(currentMap, crate);
                // draw crate
                drawEntity(crate);
            }
        }

        // Clear plr from location (note plr is 2x sprites tall)
        drawTerrain(currentMap, plr1->x, plr1->y);
        drawTerrain(currentMap, plr1->x, plr1->y - 1);
        Entity *entity_behind = entityAtLocation(entity_repo, er_length, plr1->x, plr1->y - 1);
        if(entity_behind)
            drawEntity(entity_behind);
        // Update player location
        plr1->x = nextX;
        plr1->y = nextY;
        drawEntity(plr1);


    
        if (gameSolved(currentMap, entity_repo, er_length))
        {
            Serial.println("game solved!");

            delay(200);
            gameMode = success;

            // Clear level data
            er_length = -1;

            // Prepare entityStore for next level
            currentMap++;
            if (currentMap > 1)
            {
                currentMap = 0;
                tft.drawSuccess();
            }
            else
            {
                tft.drawMapComplete(currentMap);
            }
            buildAssets(maps_20x15[currentMap]);
        }
    
    }
}
