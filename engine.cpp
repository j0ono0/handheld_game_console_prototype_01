#include "engine.h"



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

struct Entity *createEntity(struct Entity *repo, int *repo_len, enum EntityType type, int x, int y)
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

bool typeBlocksMovement(enum EntityType type)
{
    switch(type)
    {
        case stone_top_t:
        case stone_front_t:
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

bool entityBlocksMovement(struct EntityListNode *head, int x, int y)
{
    while (head != NULL)
    {
        if (head->entity->x == x && head->entity->y == y && head->entity->type == wall_t)
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

struct Entity *crateAtLocation(struct EntityListNode *head, int x, int y)
{
    while (head != NULL)
    {
        if (head->entity->x == x && head->entity->y == y && (head->entity->type == crate_t || head->entity->type == crate_active_t))
        {
            return head->entity;
        }
        head = head->next;
    }
    return NULL;
}

void deleteAssets(struct EntityListNode **assets)
{
    // struct EntityListNode *current = *assets;
    struct EntityListNode *prev = NULL;
    while (*assets != NULL)
    {
        prev = *assets;
        *assets = prev->next;
        free(prev->entity);
        free(prev);
    }
    Serial.println("finished removing assets");
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
            offsetX = STONE_TOP_X - offsetX;
            offsetY = STONE_TOP_Y - offsetY;
            break;
        case wall_t:
        case stone_top_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_TOP_X - offsetX;
            offsetY = STONE_TOP_Y - offsetY;
            break;
        case stone_front_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_FRONT_X - offsetX;
            offsetY = STONE_FRONT_Y - offsetY;
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

enum EntityType mapLocationAsTerrainType(int mapIndex, int x, int y)
{
    switch(maps_20x15[mapIndex][y][x])
    {
        case '.':
        // Floor
            return floor_t;
        case 'q':
        // Bench top end
            return bench_overhang_t;
        case 'w':
        // Bench top
            return bench_top_t;
        case 'e':
        // Bench front
            return bench_front_t;
        case '#':
            if(!terrainBlocksMovement(mapIndex, x, y+1))
                return stone_front_t;
            return stone_top_t;
        case 'N':
            return stone_front_t;
        case 'X':
        case 'B':
            return goal_t;
        default:
            // TODO: make a 'busted' sprite to show when missing sprites are returned
            return floor_t;
    }
}

bool terrainOverlays(EntityType type)
{
    switch(type)
    {

        case bench_overhang_t:
            return true;

        case stone_top_t:
        case stone_front_t:
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