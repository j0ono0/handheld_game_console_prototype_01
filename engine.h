#ifndef game_engine_h
#define game_engine_h

// #include <cstdint>
#include "Arduino.h"
#include "config.h"
#include "sprites.h"




// *IMPORTANT* The order of these must match the order of tile_ref file
enum TileRef
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

};

typedef enum 
{
    floor_t,
    water_t,
    stone_top_t,
    stone_front_t,
    stone_side_east_t,
    stone_side_west_t,
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


bool inbounds(int x, int y);
void moveSprite(int dx, int dy, Entity *entity);
bool gameSolved(int mapIndex, struct Entity *entity, int index);
struct Entity *createEntity(struct Entity *entity, int *index, EntityType type, int x, int y);
struct Entity *entityAtLocation(struct Entity *repo, int repo_len, int x, int y);
void updateCrate(int mapindex, Entity *crate);
bool terrainBlocksMovement(int mapIndex, int x, int y);
bool terrainOverlays(EntityType type);
bool typeBlocksMovement(EntityType);

bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);
EntityType mapLocationAsTerrainType(int mapIndex, int x, int y);
void spriteToBuf(const uint16_t *src, uint16_t *buf, int x, int y);

void tileToBuf(uint16_t *buf, enum TileRef tile);

void drawToBuff(uint16_t *buf, EntityType type, int offsetX, int offsetY);


int availableMemory();
#endif