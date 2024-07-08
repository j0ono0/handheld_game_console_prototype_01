/** NOTES **********************

display is 320 x 240

*******************************/

#include "game_TFT.h"
#include "input.h"
#include "engine.h"



extern const char maps_20x15[2][15][21];

// SPI hardware pins
#define TFT_DC 9
#define TFT_CS 10

Extended_Tft tft = Extended_Tft(TFT_CS, TFT_DC);


//Store all entities in a repository
Entity entity_repo[MAX_ENTITIES];

// Number of entities in repo. 
// NOTE, index of entities starts at 0, so last entity is repo_len - 1.
int repo_len;

Entity *plr1;

enum GameMode gameMode;
int currentMap;


void drawLocation(Entity *repo, int mapIndex, int x, int y)
{
    uint16_t buf[GRID_SIZE * GRID_SIZE];

    EntityType terrainType = mapLocationAsTerrainType(currentMap, x, y);

    // This draw grid cell in order of:
    // 1) Terrain floor  2) Entity  3) Terrain feature
    
    drawToBuff(buf, floor_t, 0, 0);

    drawToBuff(buf, terrainType, 0, 0);


    if(Entity *e = entityAtLocation(repo, repo_len, x, y))
    {
        drawToBuff(buf, e->type, 0, 0);
    }

    if(terrainOverlays(terrainType))
    {
        drawToBuff(buf, terrainType, 0, 0);
    }

    // It is possible another entity is overlapping this cell from in front of it
    if(Entity *e = entityAtLocation(repo, repo_len, x, y+1))
    {
        drawToBuff(buf, e->type, 0, 1);
    }

    tft.drawCellBuffer(buf, x, y);
}



void drawAssets(Entity *entity_repo, int repo_len)
{
    for(int y=0; y < GRID_HEIGHT; y++)
    {
        for(int x=0; x < GRID_WIDTH; x++)
        {
            drawLocation(entity_repo, currentMap, x, y);
        }
    }
}


void buildAssets(const char gameMap[GRID_HEIGHT][GRID_WIDTH + 1])
{
    // Output map to console (for fun?)
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
                createEntity(entity_repo, &repo_len, crate_active_t, col, row);
            }
            else if (gameMap[row][col] == "b"[0])
            {
                createEntity(entity_repo, &repo_len, crate_t, col, row);
            }
            else if (gameMap[row][col] == "@"[0])
            {
                // Assign entity as plr1 for easy reference
                plr1 = createEntity(entity_repo, &repo_len, plr_t, col, row);
            }
        }
    }
    Serial.print("entityRepo populated, count: ");
    Serial.println(repo_len);
    Serial.println("free memory: ");
    Serial.println(availableMemory());
}

void setup()
{
    Serial.begin(9600);

    setupButtonInputs();

    repo_len = 0;
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
            drawAssets(entity_repo, repo_len);
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
        struct Entity *crate = entityAtLocation(entity_repo, repo_len, nextX, nextY);
        if (crate != NULL)
        {
            if (
                !inbounds(crate->x + dx, crate->y + dy) || terrainBlocksMovement(currentMap, crate->x + dx, crate->y + dy) || entityAtLocation(entity_repo, repo_len, crate->x + dx, crate->y + dy) != NULL )
            {
                Serial.println("This crate isn't budging!");
                return;
            }
            else
            {

                // Update crate location
                crate->x += dx;
                crate->y += dy;

                // Update crate status
                updateCrate(currentMap, crate);
                // Plr has moved into old location - no need to redraw it to remove crate
                // Draw into new location
                drawLocation(entity_repo, currentMap, crate->x, crate->y);
                drawLocation(entity_repo, currentMap, crate->x, crate->y-1);

            }
        }
        
        // // Move plr

        // Update player location
        plr1->x = nextX;
        plr1->y = nextY;

        // Plr encroaches into x2 cells - both cell need redrawing

        // Remove old location
        drawLocation(entity_repo, currentMap, plr1->x - dx, plr1->y - dy);
        drawLocation(entity_repo, currentMap, plr1->x - dx, plr1->y - dy-1);
        // Add new location
        drawLocation(entity_repo, currentMap, plr1->x, plr1->y);
        drawLocation(entity_repo, currentMap, plr1->x, plr1->y-1);

    
        if (gameSolved(currentMap, entity_repo, repo_len))
        {
            Serial.println("game solved!");

            delay(200);
            gameMode = success;

            // Clear level data
            repo_len = 0;

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
