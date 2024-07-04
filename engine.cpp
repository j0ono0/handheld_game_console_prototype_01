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

struct Entity *createEntity(struct Entity *entity, int *index, enum EntityType type, int x, int y)
{
    if(*index + 1 >= MAX_ENTITIES)
        return NULL;
    // add new entity to repo
    ++*index;
    entity[*index].type = type;
    entity[*index].x = x;
    entity[*index].y = y;
    return &entity[*index];
}

struct Entity *entityAtLocation(struct Entity *entity, int index, int x, int y)
{
    for(int i = 0; i <= index; i++)
    {
        if(entity[i].x == x && entity[i].y == y)
            return &entity[i];
    }
    return NULL;
}


bool terrainBlocksMovement(int mapIndex, int x, int y)
{
    if(maps_20x15[mapIndex][y][x] == '#')
        return true;
    return false;
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

uint16_t materialColor(enum TerrainMaterial material)
{
    switch(material)
    {
        case floor_material:
            return COLOR_FLOOR;
        case goal_material:
            return COLOR_FLOOR_TARGET;
        case wall_material:
            return COLOR_WALL;
        case water_material:
            return COLOR_FLOOR;
        default:
            return 0x07e0;
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