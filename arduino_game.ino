
#include "game_TFT.h"
#include "input.h"
#include "engine.h"
#include "game_maps.h"


/** NOTES **********************

display is 320 x 240

*******************************/

#define TFT_DC 9
#define TFT_CS 10
Extended_Tft tft = Extended_Tft(TFT_CS, TFT_DC);

struct BtnHandler inputBtn
{
    .current = 0, .processed = false, .duration = 0
};

Entity *plr1;

// Store terrain by coordinates for fast and easy reference
enum TerrainMaterial terrain[GRID_WIDTH][GRID_HEIGHT];

// Store all dynamic assets into assets linked list. assetLocation tracks asset coordinates
EntityListNode *assets;

struct Entity *assetLocation[GRID_WIDTH][GRID_HEIGHT] = {NULL, };


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

void drawTerrain(int x, int y)
{
    tft.fillRect(x*GRID_SIZE, y*GRID_SIZE, GRID_SIZE, GRID_SIZE, materialColor(terrain[x][y]));
}

void drawEntity(int x, int y)
{
    if(assetLocation[x][y] != NULL)
    {
        if(assetLocation[x][y]->type == plr_t)
        {
            tft.drawPlr(x, y);
        }else{
            tft.fillRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, colorOfType(assetLocation[x][y]->type));
        }
    }
}

void drawAssets(struct EntityListNode *head)
{
    for(int y=0; y < GRID_HEIGHT; y++)
    {
        for(int x=0; x < GRID_WIDTH; x++)
        {
            drawTerrain(x, y);
            drawEntity(x, y);
        }
    }
}

void drawAsset(struct Entity *entity)
{
    if(entity->type == plr_t)
    {
        tft.drawPlr(entity->x, entity->y);
    }else{
        tft.fillRect(entity->x * GRID_SIZE, entity->y * GRID_SIZE, GRID_SIZE, GRID_SIZE, colorOfType(entity->type));
    }
}

void drawAssetAtLocation(struct EntityListNode *head, int x, int y)
{
    uint16_t color = COLOR_FLOOR;
    while (head != NULL)
    {
        if (atLocation(head->entity, x, y))
        {
            if(head->entity->type == plr_t)
            {
                tft.drawPlr(x, y);
            }
            else{
                color = colorOfType(head->entity->type);
            }
        }
        head = head->next;
    }
    tft.fillRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, color);
}

void buildAssets(char gameMap[GRID_HEIGHT][GRID_WIDTH + 1])
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

    // Populate assets and terrain
    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            if (gameMap[row][col] == "B"[0])
            {
                assetLocation[col][row] = createAsset(&assets, crate_active_t, col, row, COLOR_BOX_ACTIVE);
                terrain[col][row] = goal_material;
            }
            else if (gameMap[row][col] == "b"[0])
            {
                assetLocation[col][row] = createAsset(&assets, crate_t, col, row, COLOR_BOX);
            }
            else if (gameMap[row][col] == "#"[0])
            {
                terrain[col][row] = wall_material;
            }
            else if (gameMap[row][col] == "X"[0])
            {
                terrain[col][row] = goal_material;
            }
            else if (gameMap[row][col] == "@"[0])
            {
                // Assign entity as plr1 for easy reference
                plr1 = createAsset(&assets, plr_t, col, row, COLOR_PLAYER);
                assetLocation[col][row] = plr1;
            }
        }
    }
    Serial.println("assets build.");
    Serial.println("free memory: ");
    Serial.println(availableMemory());
}

void setup()
{

    Serial.begin(9600);

    tft.begin();
    tft.setRotation(1);

    pinMode(BTN_N, INPUT_PULLUP);
    pinMode(BTN_E, INPUT_PULLUP);
    pinMode(BTN_S, INPUT_PULLUP);
    pinMode(BTN_W, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);

    gameMode = intro;
    currentMap = 0;
    buildAssets(maps_20x15[currentMap]);
    tft.drawIntro();
}

void loop()
{
    int userInput = readUserInput(&inputBtn);

    if (gameMode != inGame)
    {
        if (userInput == 7)
        {
            Serial.println("starting game mode.");
            gameMode = inGame;
            tft.fillScreen(COLOR_FLOOR);
            drawAssets(assets);
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
        else if (terrainBlocksMovement(terrain, nextX, nextY))
        {
            Serial.println("There is no way through here.");
            return;
        }

        struct Entity *crate = assetLocation[nextX][nextY];
        if (crate)
        {
            if (
                !inbounds(crate->x + dx, crate->y + dy) || terrain[crate->x + dx][crate->y + dy] > blocking_material || assetLocation[crate->x + dx][crate->y + dy] != NULL )
            {
                Serial.println("This crate isn't budging!");
                return;
            }
            else
            {
                // Clear crate from location
                assetLocation[crate->x][crate->y] = NULL;
                drawTerrain(crate->x, crate->y);
                // Update crate location
                crate->x += dx;
                crate->y += dy;
                assetLocation[crate->x][crate->y] = crate;
                // Update crate status
                updateCrate(terrain, crate);
                // draw crate
                drawEntity(crate->x, crate->y);
            }
        }

        // Clear plr from location (note plr is 2x sprites tall)
        assetLocation[plr1->x][plr1->y] = NULL;
        drawTerrain(plr1->x, plr1->y - 1);
        drawEntity(plr1->x, plr1->y - 1);
        drawTerrain(plr1->x, plr1->y);
        // Update player location
        plr1->x = nextX;
        plr1->y = nextY;
        assetLocation[plr1->x][plr1->y] = plr1;
        drawEntity(plr1->x, plr1->y);


        if (gameSolved(terrain, assetLocation))
        {
            Serial.println("game solved!");

            delay(200);
            gameMode = success;

            // Clear arrays and delete assets
            memset(terrain, 0, sizeof(terrain));
            memset(assetLocation, 0, sizeof(assetLocation));
            deleteAssets(&assets);

            if (assets == NULL)
            {
                Serial.println("assets are NULL.");
            }
            // Prepare assets for next level
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
