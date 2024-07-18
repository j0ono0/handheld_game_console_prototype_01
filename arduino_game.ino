/** NOTES **********************

display is 320 x 240

*******************************/

#include "input.h"
#include "engine.h"


Entity *plr;

enum GameMode gameMode;

void setup()
{
    Serial.begin(9600);

    setupButtonInputs();
    screenSetup();

    setEnvironment(0);
    populateCurrentEntities();
    plr = assignPlayer();
    gameMode = intro;

    Serial.println("free memory: ");
    Serial.println(availableMemory());

}

void loop()
{
    // Track user input
    int userInput = readUserInput();
    int dx = 0;
    int dy = 0;
    // Track destination entity and tile
    Entity *target_entity = NULL;
    int nextX = plr->x;
    int nextY = plr->y;

    if (gameMode != inGame)
    {
        if (userInput == 7)
        {
            Serial.println("starting game mode.");
            gameMode = inGame;
            drawAllLocs();
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

        nextX += dx*2;
        nextY += dy*2;

        if (!inbounds(nextX, nextY))
        {
            Serial.println("Out of bounds!");
            return;
        }
        else if (terrainBlocksMovement(nextX, nextY, 2, 2))
        {
            Serial.println("There is no way through here.");
            return;
        }

        target_entity = entityAtLocation(nextX, nextY);
        Serial.println("searching for target entity...");
        if(target_entity)
        {
            Serial.println("target entity found!");
            // Test if tile after is free
            nextX += dx*2;
            nextY += dy*2;
            if(terrainBlocksMovement(nextX, nextY, 2, 2) || target_entity->type != crate_t)
            {
                Serial.println("entity blocks way.");
                // target entity cannot move, so plr cannot move
                return; 
            }
            Serial.println("moving entity.");
            moveEntity(target_entity, dx, dy);
        }
        
        Serial.println("moving plr.");
        moveEntity(plr, dx, dy);
    
        if (gameSolved())
        {
            Serial.println("game solved!");

            delay(200);
            gameMode = success;

            // Prepare entityStore for next level
            if(nextEnvironment() == 0)
                screenSuccess();
            else
                screenEnvComplete();
        }
    }

    updateSprites();
}
