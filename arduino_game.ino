
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

#define TFT_DC  9
#define TFT_CS 10
Extended_Tft tft = Extended_Tft(TFT_CS, TFT_DC);
struct BtnHandler inputBtn{ .current=0, .processed=false, .duration=0};

Entity* assetList;
Entity* player1;

Entity plr;
Entity* boxArr;
Entity* goalArr;
Entity* wallArr;
int boxCount = 0;
int goalCount = 0;
int wallCount = 0;

enum GameMode gameMode;
int currentMap;

void drawCrates(){
    for (int i=0; i < boxCount; i++){
        if(boxArr[i].color == COLOR_BOX)
        {
            tft.drawSprite(boxArr[i].x, boxArr[i].y, sprite_crate.width, sprite_crate.height, (uint16_t*)(sprite_crate.pixel_data));
        }
        else
        {
            tft.drawSprite(boxArr[i].x, boxArr[i].y, sprite_crate_active.width, sprite_crate_active.height, (uint16_t*)(sprite_crate_active.pixel_data));
        }
    }
}

void drawAllAssets(){
    tft.drawSprites(goalArr, goalCount);
    drawCrates();
    tft.drawSprites(wallArr, wallCount);
    tft.drawSprite(plr.x, plr.y, sprite_16x24_plr.width, sprite_16x24_plr.height, (uint16_t*)(sprite_16x24_plr.pixel_data));
}
void drawDynamicAssets(){
    tft.drawSprites(goalArr, goalCount);
    drawCrates();
    tft.drawSprite(plr.x, plr.y, sprite_16x24_plr.width, sprite_16x24_plr.height, (uint16_t*)(sprite_16x24_plr.pixel_data));
}

void drawAssets(struct Entity* e)
{
    tft.fillScreen(COLOR_FLOOR);
    while(e->next != NULL)
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
        tft.fillRect(e->x*GRID_SIZE, e->y*GRID_SIZE, GRID_SIZE, GRID_SIZE, e->color);
        e = e->next;
    }
}


void buildAssets(char gameMap[GRID_HEIGHT][GRID_WIDTH + 1]){


    boxCount = 0;
    goalCount = 0;
    wallCount = 0;

    // Count how large arrays need to be for game level
    for(int row = 0; row < GRID_HEIGHT; row++)
    {
        for(int col = 0; col < GRID_WIDTH; col++)
        { 
            if(gameMap[row][col] == "@"[0])
            {
                plr.x = col;
                plr.y = row;
            }
            else if(gameMap[row][col] == "B"[0])
            {
                boxCount++;
                goalCount++;
            }
            else if(gameMap[row][col] == "b"[0])
            {
                boxCount++;
            }
            else if(gameMap[row][col] == "#"[0])
            {
                wallCount++;
            }
            else if(gameMap[row][col] == "X"[0])
            {
                goalCount++;
            }

            // Output map to console
            if(gameMap[row][col] == "."[0])
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

    // Create arrays for game assets
    boxArr = (Entity*)malloc(boxCount * sizeof(struct Entity));
    goalArr = (Entity*)malloc(goalCount * sizeof(struct Entity));
    wallArr = (Entity*)malloc(wallCount * sizeof(struct Entity));
    
    // Populate array
    int boxIndex = 0;
    int goalIndex = 0;
    int wallIndex = 0;
    for(int row = 0; row < GRID_HEIGHT; row++)
    {
        for(int col = 0; col < GRID_WIDTH; col++)
        {
            if(gameMap[row][col] == "B"[0])
            {
                boxArr[boxIndex] = {.x=col, .y=row, .color=COLOR_BOX_TARGET, .type=crate_active_t};
                boxIndex++;
                goalArr[goalIndex] = {.x=col, .y=row, .color=COLOR_FLOOR_TARGET, .type=goal_t};
                goalIndex++;

                appendAsset(&assetList, col, row, goal_t, COLOR_FLOOR_TARGET);
                appendAsset(&assetList, col, row, crate_active_t, COLOR_BOX_TARGET);

            }
            else if(gameMap[row][col] == "b"[0])
            {
                boxArr[boxIndex] = {.x=col, .y=row, .color=COLOR_BOX, .type=crate_t};
                boxIndex++;

                appendAsset(&assetList, col, row, crate_t, COLOR_BOX);

            }
            else if(gameMap[row][col] == "#"[0])
            {
                wallArr[wallIndex] = {.x=col, .y=row, .color=COLOR_WALL, .type=wall_t};
                wallIndex++;

                appendAsset(&assetList, col, row, wall_t, COLOR_WALL);

            }
            else if(gameMap[row][col] == "X"[0])
            {
                goalArr[goalIndex] = {.x=col, .y=row, .color=COLOR_FLOOR_TARGET, .type=goal_t};
                goalIndex++;

                appendAsset(&assetList, col, row, goal_t, COLOR_FLOOR_TARGET);

            }else if(gameMap[row][col] == "@"[0])
            {
                plr.x = col;
                plr.y = row;
                player1 = appendAsset(&assetList, col, row, plr_t, COLOR_PLAYER);
                player1->x = col;
                player1->y = row;
            }
        }
    }
    Serial.println((unsigned int)&player1);
}

void setup() {

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

void loop() {
    int userInput = readUserInput(&inputBtn);

    if(gameMode != inGame){
        if(userInput == 7 )
        {
            Serial.println("starting game mode.");
            tft.fillScreen(COLOR_FLOOR);
            drawAllAssets();
            gameMode = inGame;
        }
        else{
            // Wait for start
            return;
        }
    }

    // Progress game
    if (userInput >= 3 && userInput <= 6){
        int nextX, nextY;
        int dx = 0;
        int dy = 0;
        switch(userInput){
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

        nextX = plr.x + dx;
        nextY = plr.y + dy;
        Entity *wallAtLocation = entityAtLocation(nextX, nextY, wallArr, wallCount);

        // Location blocks motion, end loop early
        if(!inbounds(nextX, nextY) || wallAtLocation){
            Serial.println("your way is blocked.");
            return;
        }

        Entity *boxAtLocation = entityAtLocation(nextX, nextY, boxArr, boxCount);
        
        // Location is vacant, move player
        if(!boxAtLocation)
        {
            tft.eraseSprite(plr.x, plr.y, sprite_16x24_plr.width, sprite_16x24_plr.height);
            moveSprite(dx, dy, &plr);
            drawDynamicAssets();
            return;
        }
        
        // Test if boxAtLocation can also be pushed to vacant coordinate
        nextX = boxAtLocation->x + dx;
        nextY = boxAtLocation->y + dy;        
        if(inbounds(nextX, nextY) && !entityAtLocation(nextX, nextY, boxArr, boxCount) && !entityAtLocation(nextX, nextY, wallArr, wallCount))
        {
            tft.eraseSprite(plr.x, plr.y, sprite_16x24_plr.width, sprite_16x24_plr.height);
            tft.eraseSprite(boxAtLocation->x, boxAtLocation->y, sprite_crate.width, sprite_crate.height);
            moveSprite(dx, dy, &plr);
            moveSprite(dx, dy, boxAtLocation);
            updateBoxStatus(boxAtLocation, goalArr, goalCount);
        }

        drawDynamicAssets();
        
        // Test end game criteria
        if(gameSolved(boxArr, boxCount, goalArr, goalCount))
        {
            delay(300);
            gameMode = success;
            free(boxArr);
            free(goalArr);
            free(wallArr);
            // Prepare assets for next level
            // TODO: better map progression function
            currentMap++;
            if(currentMap > 1){
                currentMap = 0;
                tft.drawSuccess();
            }else{
                tft.drawMapComplete(currentMap);
            }
            buildAssets(maps_20x15[currentMap]);
        }
    }
}
