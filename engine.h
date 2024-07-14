#ifndef game_engine_h
#define game_engine_h

#include <inttypes.h>
#include "game_TFT.h"
#include "Arduino.h"
#include "config.h"
#include "customtypes.h"
#include "sprites.h"




//////////////////////////////////////////////////////////////////
/// External graphics and environment data in resources.h       //



extern const uint16_t sprite_tile_ref_8x8[];
extern const uint16_t entity_sprites[];

extern const EnvSpec environmentList[];

//////////////////////////////////////////////////////////////////


int nextEnvironment();
int setEnvironment(int envIndex);

void populateCurrentEntities();
Entity *assignPlayer();

//////////////////////////////////////////////////////////////////
void drawAllLocs();
void blitTile(uint16_t *buf, int x, int y);
void blitTerrain(uint16_t *buf, int x, int y, int w, int h);

void drawPlr(int x, int y);
//////////////////////////////////////////////////////////////////
int tileAtLoc(int x, int y);


bool inbounds(int x, int y);

bool gameSolved();
struct Entity *entityAtLocation(int x, int y);
void updateCrate(Entity *crate);
MaterialType tileToMaterialType(TileRef tile);
bool terrainBlocksMovement(int x, int y);
bool terrainOverlays(MaterialType type);
bool typeBlocksMovement(MaterialType);

bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);

// void tileToBuf(uint16_t *buf, TileRef tile, TileLayer layer);
// void spriteToBuf(uint16_t *buf, int x, int y);


void screenSetup();
void screenDrawBuf(uint16_t *buf, int x, int y);
void screenSuccess();
void screenEnvComplete();

int availableMemory();
#endif