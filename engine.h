#ifndef game_engine_h
#define game_engine_h

#include <inttypes.h>
#include "game_TFT.h"
#include "Arduino.h"
#include "config.h"
#include "customtypes.h"




//////////////////////////////////////////////////////////////////
/// External graphics and environment data in resources.h       //

extern const uint16_t title_screen_main[];
extern const uint16_t title_screen_section[];

extern const uint16_t entity_sprites_2[];

extern const uint16_t sprite_tile_ref_8x8[];
extern const uint16_t sprite_8x8_overlays[];
extern const uint16_t entity_sprites[];
extern const EnvSpec environmentList[];

//////////////////////////////////////////////////////////////////

int nextEnvironment();
int setEnvironment(int envIndex);

void populateCurrentEntities();
Entity *assignPlayer();

//////////////////////////////////////////////////////////////////
void advanceSpriteAnimations();
void drawAll();
void blitOverlay(int x, int y, int w, int h, uint16_t *buf);
void blitTerrain(int x, int y, int w, int h, uint16_t *buf);
void sortEntityDrawOrder();
void drawEntities(int layer);
void blitEntity(Entity *e, uint16_t *buf);

void moveEntity(Entity *e, int mx, int my);

//////////////////////////////////////////////////////////////////

void updateSprites();
bool spritesInTransit();

//////////////////////////////////////////////////////////////////
int tileAtLoc(int x, int y);

bool inbounds(int x, int y);

bool gameSolved();
struct Entity *entityAtLocation(int x, int y, int layer);
bool terrainBlocksMovement(int x, int y, int w, int h);

bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);


void screenSetup();
void screenDrawBuf(uint16_t *buf, int x, int y);
void screenSuccess();
void screenEnvComplete();

///////////////////////////////////////////////////////////////

void runBehaviours();
void do_nothing(Entity *e);
void act_test(Entity *e);
bool entity_on_target(Entity *e);


int availableMemory();
#endif