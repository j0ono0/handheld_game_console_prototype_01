
#include "game_TFT.h"
#include "input.h"
#include "engine.h"
#include "game_maps.h"

#include "sprite_plr.c"
#include "sprite_16x24_plr.c"
#include "sprite_crate.c"
#include "sprite_crate_active.c"

/** NOTES **********************

TFT_DC LOW = sending command byte
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
EntityListNode *assets;

enum GameMode gameMode;
int currentMap;

void drawAssets(struct EntityListNode *head)
{
    while (head != NULL)
    {
        // switch(e->type){
        //     case plr_t:
        //         tft.drawSprite(e->x, e->y, sprite_16x24_plr.width, sprite_16x24_plr.height, (uint16_t*)(sprite_16x24_plr.pixel_data));
        //         break;
        //     case crate_t:
        //         tft.drawSprite(e->x, e->y, sprite_crate.width, sprite_crate.height, (uint16_t*)(sprite_crate.pixel_data));
        //         break;
        //     case crate_active_t:
        //         tft.drawSprite(e->x, e->y, sprite_crate_active.width, sprite_crate_active.height, (uint16_t*)(sprite_crate_active.pixel_data));
        //         break;
        //     case goal_t:
        //         tft.drawRect(e->x*GRID_SIZE, e->y*GRID_SIZE, GRID_SIZE, GRID_SIZE, e->color);
        //         break;
        //     case wall_t:
        //         tft.fillRect(e->x*GRID_SIZE, e->y*GRID_SIZE, GRID_SIZE, GRID_SIZE, e->color);
        //         break;
        //     default:
        //         break;
        // }
        uint16_t color = 0x0;
        switch (head->entity->type)
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
        tft.fillRect(head->entity->x * GRID_SIZE, head->entity->y * GRID_SIZE, GRID_SIZE, GRID_SIZE, color);
        head = head->next;
    }
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

    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            if (gameMap[row][col] == "B"[0])
            {
                createAsset(&assets, goal_t, col, row, COLOR_FLOOR_TARGET);
                createAsset(&assets, crate_active_t, col, row, COLOR_BOX_ACTIVE);
            }
            else if (gameMap[row][col] == "b"[0])
            {
                createAsset(&assets, crate_t, col, row, COLOR_BOX);
            }
            else if (gameMap[row][col] == "#"[0])
            {
                createAsset(&assets, wall_t, col, row, COLOR_WALL);
            }
            else if (gameMap[row][col] == "X"[0])
            {
                createAsset(&assets, goal_t, col, row, COLOR_FLOOR_TARGET);
            }
            else if (gameMap[row][col] == "@"[0])
            {
                // Assign entity as plr1 for easy reference
                plr1 = createAsset(&assets, plr_t, col, row, COLOR_PLAYER);
            }
        }
    }
    Serial.println("assets build.");
    Serial.println("free memory: ");
    Serial.println(freeMemory());
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
            return;
        }
        else
        {
            // Wait for start
            return;
        }
    }

    drawAssets(assets);

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
        else if (entityBlocksMovement(assets, nextX, nextY))
        {
            Serial.println("A wall blocks your way.");
            return;
        }

        struct Entity *crate = crateAtLocation(assets, nextX, nextY);
        if (crate)
        {
            if (
                !inbounds(crate->x + dx, crate->y + dy) || entityBlocksMovement(assets, crate->x + dx, crate->y + dy) || crateAtLocation(assets, crate->x + dx, crate->y + dy))
            {
                Serial.println("This crate isn't budging!");
                return;
            }
            else
            {
                crate->x += dx;
                crate->y += dy;
                updateCrate(assets, crate);
            }
        }
        tft.fillRect(plr1->x * GRID_SIZE, plr1->y * GRID_SIZE, GRID_SIZE, GRID_SIZE, COLOR_FLOOR);
        plr1->x = nextX;
        plr1->y = nextY;

        if (gameSolved(assets))
        {
            Serial.println("game solved!");

            delay(200);
            gameMode = success;

            // TODO: THis does not work properly I think!!!!
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
