/** NOTES **********************

display is 320 x 240

*******************************/

#include "input.h"
#include "engine.h"


Entity *plr1;

enum GameMode gameMode;


void setup()
{
    Serial.begin(9600);

    setupButtonInputs();
    screenSetup();

    setEnvironment(0);
    populateCurrentEntities();
    plr1 = assignPlayer();
    gameMode = intro;

    Serial.println("free memory: ");
    Serial.println(availableMemory());

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
        else if (terrainBlocksMovement(nextX, nextY))
        {
            Serial.println("There is no way through here.");
            return;
        }
        struct Entity *crate = entityAtLocation(nextX, nextY);
        if (crate != NULL)
        {
            if (
                !inbounds(crate->x + dx, crate->y + dy) || terrainBlocksMovement(crate->x + dx, crate->y + dy) || entityAtLocation(crate->x + dx, crate->y + dy) != NULL )
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
                updateCrate(crate);
                // Plr has moved into old location - no need to redraw it to remove crate
                // Draw into new location
                drawLoc(crate->x, crate->y);
                drawLoc(crate->x, crate->y-1);

            }
        }
        
        // // Move plr

        // Update player location
        plr1->x = nextX;
        plr1->y = nextY;

        // Plr encroaches into x2 cells - both cell need redrawing

        // Remove old location
        drawLoc(plr1->x - dx, plr1->y - dy-1);
        drawLoc(plr1->x - dx, plr1->y - dy);
        // Add new location
        drawLoc(plr1->x, plr1->y);
        drawLoc(plr1->x, plr1->y-1);

    
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
}
