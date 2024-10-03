#ifndef game_engine_h
#define game_engine_h

#include <inttypes.h>
#include "Arduino.h"
#include "game_TFT.h"
#include "config.h"
#include "customtypes.h"
#include "interactions.h"


void touchInit();
void sdCardTest();
void screenSetup();
//////////////////////////////////////////////////////////////////

void populateEntities(const Entity *entities, uint8_t length);
Entity *assignPlayer();
void setTerrain(const uint8_t *terrain);
void setPalette(const uint8_t palette);

//////////////////////////////////////////////////////////////////
void advanceSpriteAnimations();
void drawAll();
void blitTerrain(uint8_t layer, uint16_t *buf);
// uint16_t get_indexed_color(int color_index, int theme_index); // OBSELETE?
void sortEntityDrawOrder();
void drawEntities(int layer);
void blitEntity(Entity *e, uint16_t *buf);

void moveEntity(Entity *e, int mx, int my);

//////////////////////////////////////////////////////////////////
bool advance_animation_clock(uint16_t *clock);
void updateSpriteTransits();
bool spritesInTransit();

//////////////////////////////////////////////////////////////////

bool inbounds(int x, int y);

bool cheatGame(int keypress);
bool gameSolved();


void screenDrawBuf(uint16_t *buf, int x, int y);
void screenIntro();
void screenDemo1();
void screenSuccess();
void screenEnvComplete();


void testTouchscreen();

///////////////////////////////////////////////////////////////

void runBehaviours();
bool entity_on_target(Entity *e, GameManager *gm);

///////////////////////////////////////////////////////////////


#endif