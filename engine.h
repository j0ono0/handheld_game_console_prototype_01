#ifndef game_engine_h
#define game_engine_h

#include <inttypes.h>
#include "Arduino.h"
#include "game_TFT.h"
#include "config.h"
#include "sprites.h"


// *IMPORTANT* The order of these must match the order of tile_ref file
typedef enum
{
    // Bases
    missing_tr,
    floor_tr,
    stone_tr,
    water_tr,
    // Features
    stone_front_tr,
    stone_w_tr,
    stone_e_tr,
    stone_nw_tr,
    stone_ne_tr,
    stone_sw_tr,
    stone_se_tr,
    // Compound 
    floor_stone_overhang_tr,
    floor_target_tr,
    water_stone_overhang_tr,
    water_target_tr,

} TileRef;

typedef enum 
{
    floor_t,
    water_t,
    stone_t,
    stone_front_t,
    stone_w_t,
    stone_e_t,
    stone_ne_t,
    stone_nw_t,
    stone_sw_t,
    stone_se_t,
    stone_overhang_t,
    bench_front_t,
    bench_top_t,
    bench_overhang_t,
    goal_t,
    wall_t,
    plr_t,
    crate_t,
    crate_active_t,
} EntityType;

enum GameMode
{
    intro,
    inGame,
    success
};

typedef struct Entity
{
    int x;
    int y;
    EntityType type;
} Entity;

//////////////////////////////////////////////////////////////////
////// External resources for reference in engine               //

extern const uint16_t sprite_sheet_01[];
extern const uint8_t terrain_0[];
extern const uint8_t terrain_1[];

//////////////////////////////////////////////////////////////////


int currentEntitiesLength(void);

int nextEnvironment();
int setEnvironment(int envIndex);

void populateCurrentEntities();
Entity *assignPlayer();

//////////////////////////////////////////////////////////////////
void drawAllLocs();
void drawLoc(int x, int y);

//////////////////////////////////////////////////////////////////
int tileAtLoc(int x, int y);


bool inbounds(int x, int y);
void moveSprite(int dx, int dy, Entity *entity);
bool gameSolved();
struct Entity *entityAtLocation(int x, int y);
void updateCrate(Entity *crate);
EntityType tileToEntityType(TileRef tile);
bool terrainBlocksMovement(int x, int y);
bool terrainOverlays(EntityType type);
bool typeBlocksMovement(EntityType);

bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);

void tileToBuf(uint16_t *buf, TileRef tile);
void drawToBuff(uint16_t *buf, EntityType type, int offsetX, int offsetY);

void spriteToBuf(uint16_t *buf, int x, int y);

void screenSetup();
void screenDrawBuf(uint16_t *buf, int x, int y);
void screenSuccess();
void screenEnvComplete();

int availableMemory();
#endif