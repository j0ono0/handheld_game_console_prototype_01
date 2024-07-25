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

unsigned long throttle = 0;

void loop()
{
    // Track user input
    int userInput = readUserInput();

    // Add input to queue
    if (userInput != -1)
    {
        enqueue_kpq(userInput);
    }

    // Wait for animations to finish but
    // restart loop to enqueue more user input
    if(spritesInMotion())
    {
        updateSprites();
        drawAll();
        return;
    }

    if(millis() - throttle > 1000)
    {
        throttle = millis();
    }else{
        return;
    }


    // Get next user input from queue
    int next_input = dequeue_kpq();

    if(next_input != -1){
        Serial.print("next input cmd: ");
        Serial.println(next_input);
    }

    int dx = 0;
    int dy = 0;
    // Track destination entity and tile
    // Entity *target_entity = NULL;
    int nextX = plr->x;
    int nextY = plr->y;

    if (gameMode != inGame)
    {
        if (next_input == 7)
        {
            Serial.println("starting game mode.");
            gameMode = inGame;
            drawAll();
            return;
        }
        else
        {
            // Wait for start
            return;
        }
    }

    // Progress game
    if (next_input >= 3 && next_input <= 6)
    {
        switch (next_input)
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

        nextX += dx;
        nextY += dy;

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

        Entity *target_entity = entityAtLocation(nextX, nextY);
        if(target_entity)
        {
            // Test if tile after is free
            nextX += dx;
            nextY += dy;

            if(terrainBlocksMovement(nextX, nextY, 2, 2))
            {
                return;
            }

            Entity *entity_at_next_loc = entityAtLocation(nextX, nextY);

            if(entity_at_next_loc && entity_at_next_loc->type != target_t)
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
    
        runBehaviours();

    }
    updateSprites();
    drawAll();

    if (gameSolved())
    {

        // Complete movement animations
        while(spritesInMotion())
        {         
            updateSprites();
            drawAll();
        }

        gameMode = success;
        Serial.println("game solved!");
        delay(200);

        // Prepare entityStore for next level
        if(nextEnvironment() == 0)
            screenSuccess();
        else
            screenEnvComplete();
    }
}
