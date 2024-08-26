
#include <cstddef>
#include "interactions.h"

extern const EnvSpec environmentList[];

// This is auto-generated via python script
// Order matches tileset order
const TileMeta tile_meta[] = {
    {0, false},
    {0, false},
    {3, false},
    {3, false},
    {3, false},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, false},
    {0, false},
    {0, false},
    {0, false},
    {0, false},
    {0, false},
    {0, false},
    {0, false},
    {0, false},
    {0, true},
    {3, false},
    {3, false},
    {3, false},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
    {0, true},
};

bool inbounds(int x, int y)
{
    // Note: x and y in ENV_UNITS
    if (x < 0 || y < 0 || x >= ENV_WIDTH || y >= ENV_HEIGHT)
    {
        return false;
    }
    return true;
}

int tileAtLoc(int x, int y, GameManager *gm)
{
    return gm->terrain[y * TERRAIN_WIDTH + x];
}

bool atLocation(Entity *entity, int x, int y)
{
    if (entity->x == x && entity->y == y)
    {
        return true;
    }
    return false;
}

bool coLocated(Entity *a, Entity *b)
{
    if (a->x == b->x && a->y == b->y)
    {
        return true;
    }
    return false;
}

bool terrainBlocksMovement(int x, int y, int w, int h, GameManager *gm)
{
    // x and y are ENV_UNITS, convert to TERRAIN_UNITS
    int tx = x * ENV_UNIT / TERRAIN_UNIT;
    int ty = y * ENV_UNIT / TERRAIN_UNIT;
    // Width and height are size of entity footprint
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (tile_meta[tileAtLoc(tx + i, ty + j, gm)].blocks_motion)
                return true;
        }
    }
    return false;
}

struct Entity *entityAtLocation(int x, int y, int layer, GameManager *gm)
{
    for (int i = 0; i < gm->e_len; ++i)
    {
        if (
            gm->entities[i].x == x &&
            gm->entities[i].y == y &&
            gm->entities[i].layer == layer)
            return &gm->entities[i];
    }
    return NULL;
}

void moveEntity(Entity *e, int dx, int dy)
{

    // Update entity (x, y) position
    // *** NOTE: entity coordinates updated at immediately:
    // So subsequent entities moving interact with an up-to-date environment ***
    e->x += dx;
    e->y += dy;

    // mx and my are in pixels.
    // they indicate direction and distance sprite has to move
    e->mx += dx * ENV_UNIT;
    e->my += dy * ENV_UNIT;
}

bool entity_on_target(Entity *e, GameManager *gm)
{
    for (int i = 0; i < gm->e_len; ++i)
    {
        if (gm->entities[i].type == target_t && coLocated(e, &gm->entities[i]))
        {
            return true;
        }
    }
    return false;
}