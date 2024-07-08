#ifndef game_engine_h
#define game_engine_h

#include <cstdint>
#include "Arduino.h"
#include "config.h"
#include "sprites.h"

// Not sure if this is the right place to include maps const?
#include "game_maps.c"

enum TerrainMaterial
{
    floor_material,
    goal_material,
    blocking_material, //Everything past here blocks motion
    wall_material,
    water_material
};

enum EntityType
{
    floor_t,
    water_t,
    bench_front_t,
    bench_top_t,
    bench_overhang_t,
    goal_t,
    wall_t,
    plr_t,
    crate_t,
    crate_active_t,
};

#define OVERLAYTERRAINTYPE_LENGTH 1
const EntityType overlayTerrainType[] = {
    bench_overhang_t,
};

enum GameMode
{
    intro,
    inGame,
    success
};

struct Entity
{
    int x;
    int y;
    enum EntityType type;
};

// Linked lists
struct EntityListNode
{
    struct Entity *entity;
    struct EntityListNode *next;
};

bool inbounds(int x, int y);
void moveSprite(int dx, int dy, Entity *entity);
bool gameSolved(int mapIndex, struct Entity *entity, int index);
struct Entity *createEntity(struct Entity *entity, int *index, enum EntityType type, int x, int y);
struct Entity *entityAtLocation(struct Entity *repo, int repo_len, int x, int y);
void updateCrate(int mapindex, Entity *crate);
bool terrainBlocksMovement(int mapIndex, int x, int y);

bool entityBlocksMovement(struct EntityListNode *head, int x, int y);
struct Entity *crateAtLocation(struct EntityListNode *head, int x, int y);
void deleteAssets(struct EntityListNode **assets);
bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);
uint16_t materialColor(enum TerrainMaterial);
void drawToBuff(uint16_t *buf, EntityType type, int offsetX, int offsetY);
enum EntityType mapLocationAsTerrainType(int mapIndex, int x, int y);
bool terrainOverlays(EntityType type);
int availableMemory();
#endif