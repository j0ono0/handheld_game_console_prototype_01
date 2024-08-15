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
    Serial.println("hello serial?");
    setupButtonInputs();
    screenSetup();

    setEnvironment(0);
    populateCurrentEntities();
    plr = assignPlayer();
    gameMode = intro;

}

void loop()
{

    // Collect user input   //////////////////////////////////////////////
    
    // Queue user keypress input
    enqueue_kpq(readUserInput());

    // Complete existing entity transits
    if(spritesInTransit())
    {
        advanceSpriteAnimations();
        return;
    }


    // Wait to start game  //////////////////////////////////////////////

    if (gameMode != inGame)
    {
        if (dequeue_kpq() == 7)
        {
            Serial.println("starting game mode.");
            gameMode = inGame;
            drawAll();
            return;
        }
        else
        {
            // Waiting for start
            return;
        }
    }

    // Test for end game  //////////////////////////////////////////////

    if (gameSolved())
    {
        // Draw final stationary sprites
        advanceSpriteAnimations();

        gameMode = success;
        Serial.println("game solved!");
        delay(200);

        // Prepare entityStore for next level
        if(nextEnvironment() == 0)
            screenSuccess();
        else
            screenEnvComplete();
        
        // Return to loop start
        return;
    }


    // Get next keypress input from queue
    int next_input = dequeue_kpq();

    // Do another render. 
    // Entity may have change from in-transit to stationary sprites
    advanceSpriteAnimations();
    
    // Plr takes action, then other entities take action  /////////////////////

    int dx = 0;
    int dy = 0;
    // Track destination entity and tile
    // Entity *target_entity = NULL;
    int nextX = plr->x;
    int nextY = plr->y;

    // Progress game

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
    case BTN_SELECT:
        // dev feature! Abort current env and start next one.
        nextEnvironment();
        break;
    default:
     // Restart loop - no user input.
        return;
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

    Entity *target_entity = entityAtLocation(nextX, nextY, plr->layer);
    if(target_entity && target_entity->layer == plr->layer)
    {
        // Test if tile after is free
        nextX += dx;
        nextY += dy;

        if(terrainBlocksMovement(nextX, nextY, 2, 2))
        {
            return;
        }

        Entity *entity_at_next_loc = entityAtLocation(nextX, nextY, target_entity->layer);

        if(entity_at_next_loc && entity_at_next_loc->layer == target_entity->layer)
        {
            Serial.println("entity blocks way.");
            // target entity cannot move, so plr cannot move
            return; 
        }
        moveEntity(target_entity, dx, dy);
    }

    moveEntity(plr, dx, dy);

    runBehaviours();

}
