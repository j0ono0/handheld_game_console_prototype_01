#include "engine.h"


// Not sure if this is the right place to include maps const?
#include "game_maps.c"
extern const char maps_20x15[2][15][21];

bool inbounds(int x, int y)
{
    if (x < 0 || y < 0 || x > GRID_WIDTH || y > GRID_HEIGHT)
    {
        return false;
    }
    return true;
}

void moveSprite(int dx, int dy, Entity *entity)
{
    entity->x += dx;
    entity->y += dy;
}

void updateCrate(int mapIndex, Entity *crate)
{
    switch(maps_20x15[mapIndex][crate->y][crate->x])
    {
        case 'X':
        case 'B':
            crate->type = crate_active_t;
            return;
        default:
            crate->type = crate_t;
            return;
    }
}

bool gameSolved(int mapIndex, struct Entity *entity, int index)
{
    // Test if every goal has a crate on the same location.
    for(int y = 0; y < GRID_HEIGHT; y++)
    {
        for(int x = 0; x < GRID_WIDTH; x++)
        {
            if(maps_20x15[mapIndex][y][x] == 'X' || maps_20x15[mapIndex][y][x] == 'B')
            {
                Entity *e = entityAtLocation(entity, index, x, y);
                if(e == NULL || e->type != crate_active_t)
                    return false;
            }
        }
    }
    return true;
}

struct Entity *createEntity(struct Entity *repo, int *repo_len, EntityType type, int x, int y)
{
    if(*repo_len  > MAX_ENTITIES)
        return NULL;
    // add new entity to repo
    int i = *repo_len;
    *repo_len = *repo_len + 1;
    repo[i].type = type;
    repo[i].x = x;
    repo[i].y = y;
    return &repo[i];
}

struct Entity *entityAtLocation(struct Entity *repo, int repo_len, int x, int y)
{
    for(int i = 0; i < repo_len; i++)
    {
        if(repo[i].x == x && repo[i].y == y)
            return &repo[i];
    }
    return NULL;
}

bool terrainBlocksMovement(int mapIndex, int x, int y)
{
    return typeBlocksMovement(mapLocationAsTerrainType(mapIndex, x, y));
}

bool typeBlocksMovement(EntityType type)
{
    switch(type)
    {
        case stone_top_t:
        case stone_front_t:
        case stone_side_east_t:
        case stone_side_west_t:
        case bench_front_t:
        case bench_top_t:
        case crate_t:
        case crate_active_t:
        case plr_t:
        case wall_t:
            return true;

        case bench_overhang_t:
        case floor_t:
        case goal_t:
        case water_t:
            return false;

        default:
            return false;
    }
}

bool terrainOverlays(EntityType type)
{
    switch(type)
    {

        case stone_overhang_t:
        case bench_overhang_t:
            return true;

        case stone_top_t:
        case stone_front_t:
        case stone_side_east_t:
        case stone_side_west_t:

        case bench_front_t:
        case bench_top_t:
        case crate_t:
        case crate_active_t:
        case plr_t:
        case wall_t:
        case floor_t:
        case goal_t:
        case water_t:
            return false;
            
        default:
            return false;
    }
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

EntityType mapLocationAsTerrainType(int mapIndex, int x, int y)
{
    switch(maps_20x15[mapIndex][y][x])
    {
        case '.':
        case '@':
            return floor_t;
        case 'q':
            return bench_overhang_t;
        case 'w':
            return bench_top_t;
        case 'e':
            return bench_front_t;
        case '#':           
            return stone_top_t;
        case 'N':
            return stone_front_t;
        case 'W':
            return stone_side_west_t;
        case 'S':
            return stone_overhang_t;
        case 'E':
            return stone_side_east_t;
        case 'X':
        case 'B':
            return goal_t;
        default:
            // TODO: make a 'busted' sprite to show when missing sprites are returned
            return floor_t;
    }
}

void spriteToBuf(uint16_t *buf, int offset)
{
    const uint16_t *pixelPtr = &sprite_sheet_01[offset];
    for(int y = 0; y < GRID_SIZE; y++)
    {
        for(int x=0; x < GRID_SIZE; x++)
        {
            if(*pixelPtr != COLOR_TRANSPARENT)
                buf[y * GRID_SIZE + x] = *pixelPtr;
            ++pixelPtr;
        }
        pixelPtr += SPRITESHEET_WIDTH - GRID_SIZE ;
    }
}

void tileToBuf(uint16_t *buf, enum TileRef tile)
{
    EntityType type = floor_t;
    switch(tile)
    {
        case floor_tr:
            type = floor_t;
            break;
        case stone_tr:
            type = stone_top_t;
            break;
        // case water_tr:
        //     type = floor_t;
        //     break;
        // Features
        case stone_front_tr:
            type = stone_front_t;
            break;
        case stone_w_tr:
            type = stone_side_west_t;
            break;
        case stone_e_tr:
            type = stone_side_east_t;
            break;
        // case stone_nw_tr:
        //     type = floor_t;
        //     break;
        // case stone_ne_tr:
        //     type = floor_t;
        //     break;
        // case stone_sw_tr:
        //     type = floor_t;
        //     break;
        // case stone_se_tr:
        //     type = floor_t;
        //     break;
        // Compound 
        case floor_stone_overhang_tr:
            drawToBuff(buf, floor_t, 0, 0);
            type = stone_overhang_t;
            break;
        case floor_target_tr:
            drawToBuff(buf, floor_t, 0, 0);
            type = goal_t;
            break;
        // case water_stone_overhang_tr:
        //     type = floor_t;
        //     break;
        // case water_target_tr:
        //     type = floor_t;
        //     break;
        default: 
            type = floor_t;
            break;
    }
    drawToBuff(buf, type, 0, 0);
}

void drawToBuff(uint16_t *buf, EntityType type, int offsetX, int offsetY)
{
    switch(type)
    {
        case floor_t:
        case water_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = FLOOR_X - offsetX;
            offsetY = FLOOR_Y - offsetY;
            break;
        case goal_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = GOAL_X - offsetX;
            offsetY = GOAL_Y - offsetY;
            break;

        case wall_t:
        case stone_top_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_TOP_X - offsetX;
            offsetY = STONE_TOP_Y - offsetY;
            break;

        case stone_side_east_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_SIDE_EAST_X - offsetX;
            offsetY = STONE_SIDE_EAST_Y - offsetY;
                break;
            
        case stone_side_west_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_SIDE_WEST_X - offsetX;
            offsetY = STONE_SIDE_WEST_Y - offsetY;
                break;
            
        case stone_front_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_FRONT_X - offsetX;
            offsetY = STONE_FRONT_Y - offsetY;
                break;

        case stone_overhang_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_OVERHANG_X - offsetX;
            offsetY = STONE_OVERHANG_Y - offsetY;
            break;

        case bench_overhang_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_OVERHANG_X - offsetX;
            offsetY = STONE_OVERHANG_Y - offsetY;
            break;

        case bench_top_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_TOP_X - offsetX;
            offsetY = STONE_TOP_Y - offsetY;
            break;

        case bench_front_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_FRONT_X - offsetX;
            offsetY = STONE_FRONT_Y - offsetY;
            break;

        case plr_t:
            // Do nothing if no part of plr sprite falls inside buffer
            if(offsetX != 0 || offsetY < 0 || offsetY > 1)
                return;
            offsetX = PLR_X - offsetX;
            offsetY = PLR_Y - offsetY;
            break;

        case crate_t:
            if(offsetX != 0 || offsetY < 0 || offsetY > 1)
                return;
            offsetX = CRATE_X - offsetX;
            offsetY = CRATE_Y - offsetY;
            break;

        case crate_active_t:
            if(offsetX != 0 || offsetY < 0 || offsetY > 1)
                return;
            offsetX = CRATE_ACTIVE_X - offsetX;
            offsetY = CRATE_ACTIVE_Y - offsetY;
            break;

    };
    spriteToBuf(buf, (offsetY) * GRID_SIZE * SPRITESHEET_WIDTH + offsetX * GRID_SIZE);
}




/////////////////////////////////////////////////////////
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int availableMemory()
{
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}