
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


enum GameMode gameMode;
int currentMap;



void drawAssets(struct Entity* e)
{
    while(e != NULL)
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

    // Output map to console
    for(int row = 0; row < GRID_HEIGHT; row++)
    {
        for(int col = 0; col < GRID_WIDTH; col++)
        { 
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

    for(int row = 0; row < GRID_HEIGHT; row++)
    {
        for(int col = 0; col < GRID_WIDTH; col++)
        {
            if(gameMap[row][col] == "B"[0])
            {
                appendAsset(&assetList, col, row, goal_t, COLOR_FLOOR_TARGET);
                appendAsset(&assetList, col, row, crate_active_t, COLOR_BOX_ACTIVE);

            }
            else if(gameMap[row][col] == "b"[0])
            {
                appendAsset(&assetList, col, row, crate_t, COLOR_BOX);

            }
            else if(gameMap[row][col] == "#"[0])
            {
                appendAsset(&assetList, col, row, wall_t, COLOR_WALL);

            }
            else if(gameMap[row][col] == "X"[0])
            {
                appendAsset(&assetList, col, row, goal_t, COLOR_FLOOR_TARGET);

            }else if(gameMap[row][col] == "@"[0])
            {
                player1 = appendAsset(&assetList, col, row, plr_t, COLOR_PLAYER);
                player1->x = col;
                player1->y = row;
                player1->color = COLOR_PLAYER;
            }
        }
    }
    Serial.println("assets build.");
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
            gameMode = inGame;
            tft.fillScreen(COLOR_FLOOR);
            return;
        }
        else{
            // Wait for start
            return;
        }
    }
    
    drawAssets(assetList);

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

        nextX = player1->x + dx;
        nextY = player1->y + dy;

        if(!inbounds(nextX, nextY)){
            Serial.println("Out of bounds!");
            return;
        }

        Entity *targetEntity;
        targetEntity = assetAtLocation(nextX, nextY, assetList);      
       
        // Location is vacant, move player
        if(!targetEntity || targetEntity->type == goal_t)
        {
            tft.fillRect(player1->x*GRID_SIZE, player1->y*GRID_SIZE, GRID_SIZE, GRID_SIZE, COLOR_FLOOR);
            moveSprite(dx, dy, player1);
            return;
        }

        if(targetEntity->type == wall_t)
        {
            Serial.println("That way is blocked!");
            return;
        }


        // Test if targetEntity can also be pushed to vacant coordinate
        nextX = targetEntity->x + dx;
        nextY = targetEntity->y + dy;
        struct Entity* nextBlockingAsset =  assetAtLocation(nextX, nextY, assetList);
        if(inbounds(nextX, nextY) && (!nextBlockingAsset || nextBlockingAsset->type == goal_t))
        {
            tft.fillRect(player1->x*GRID_SIZE, player1->y*GRID_SIZE, GRID_SIZE, GRID_SIZE, COLOR_FLOOR);
            tft.eraseSprite(targetEntity->x, targetEntity->y, sprite_crate.width, sprite_crate.height);
            moveSprite(dx, dy, player1);
            moveSprite(dx, dy, targetEntity);
            if(nextBlockingAsset->type == goal_t){
                targetEntity->type = crate_active_t;
            }
                targetEntity->type = crate_t;
            // updateBoxStatus(boxAtLocation, goalArr, goalCount);
        }

        
        // Test end game criteria
        // if(gameSolved(assetList))
        // {
        //     drawAssets(assetList);
        //     delay(300);
        //     gameMode = success;
        //     // Prepare assets for next level
        //     // TODO: better map progression function
        //     currentMap++;
        //     if(currentMap > 1){
        //         currentMap = 0;
        //         tft.drawSuccess();
        //     }else{
        //         tft.drawMapComplete(currentMap);
        //     }
        //     buildAssets(maps_20x15[currentMap]);
        // }
    }
}
