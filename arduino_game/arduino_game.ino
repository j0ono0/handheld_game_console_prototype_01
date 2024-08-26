/** NOTES **********************

display is 320 x 240

*******************************/

#include "input.h"
#include "engine.h"

#include "resources.h"

uint8_t current_env;

void setup()
{
    Serial.begin(9600);
    setupButtonInputs();
    screenSetup();

    // setEnvironment(0);
    current_env = 0;
    populateEntities(environmentList[current_env].entities, environmentList[current_env].entity_count);
    setTerrain(environmentList[current_env].terrain);
    setGameMode(gm_intro);
}

void loop()
{

    // Collect user input   //////////////////////////////////////////////

    // Queue user keypress input
    enqueue_kpq(readUserInput());

    // Complete existing entity transits
    if (spritesInTransit())
    {
        advanceSpriteAnimations();
        return;
    }

    // Wait to start game  //////////////////////////////////////////////

    if (gameMode() != gm_inGame)
    {
        if (dequeue_kpq() == 7)
        {
            if (gameMode() == gm_end)
            {
                // cycle game back to splashscreen
                setGameMode(gm_intro);
                screenIntro();
                return;
            }
            else
            {
                // Commence new game play
                Serial.println("starting game mode.");
                setGameMode(gm_inGame);
                drawAll();
                return;
            }
        }
        else
        {
            // Waiting for start
            return;
        }
    }

    // Get next keypress input from queue
    // int next_input = dequeue_kpq();

    // Test for end game  //////////////////////////////////////////////

    if (gameSolved())
    {
        // Draw final stationary sprites
        advanceSpriteAnimations();

        Serial.println("game solved!");
        delay(200);

        // Prepare entityStore for next level (or restart)
        if (current_env == 0)
        {
            setGameMode(gm_end);
            ++current_env;
            populateEntities(environmentList[current_env].entities, environmentList[current_env].entity_count);
            setTerrain(environmentList[current_env].terrain);
            screenSuccess();
        }
        else
        {
            setGameMode(gm_success);
            current_env = 0;
            screenEnvComplete();
        }

        // Return to loop start
        return;
    }

    // Do another render.
    // Entity may have change from in-transit to stationary sprites
    advanceSpriteAnimations();

    runBehaviours();
    sortEntityDrawOrder();
}
