#include "engine.h"

Extended_Tft screen = Extended_Tft(TFT_CS, TFT_DC);

// Current environment
int envId = 0;

// Entities currently in game.
Entity currentEntities[MAX_ENTITIES];
int currentEntityLength = 0;

/////////////////////////////////////////////////////

int TEMP_ENTITY_COUNT = 8;

void populateCurrentEntities()
{
    currentEntityLength = 0;
    for(int i = 0; i < TEMP_ENTITY_COUNT; ++i)
    {
        currentEntities[currentEntityLength++] = environmentList[envId].entities[i];
    }
}

Entity *assignPlayer()
{
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].type == plr_t)
            return &currentEntities[i];
    }
    return NULL;
}

/////////////////////////////////////////////////////
// Env controls /////////////////////////////////////

int nextEnvironment()
{
    return setEnvironment(++envId);
}
int setEnvironment(int envIndex)
{
    // TODO: ensure index cannot be outside terrainList
    if(envIndex > 1)
        envIndex = 0;
    envId = envIndex;
    populateCurrentEntities();
    return envId;
}
/////////////////////////////////////////////////////


void drawAllLocs()
{
     for(int y=0; y < GRID_HEIGHT; y++)
    {
        for(int x=0; x < GRID_WIDTH; x++)
        {
            drawLoc(x, y);
        }
    }
}

void drawLoc(int x, int y)
{
    uint16_t buf[GRID_SIZE * GRID_SIZE];
    // Draw tile

    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    // trying out replacement tiles       ////////////////////////
    uint16_t tileId = environmentList[envId].terrain[y * GRID_WIDTH + x];
    const uint16_t *pixelPtr = &sprite_tile_ref_01[GRID_SIZE * GRID_SIZE * tileId];
    uint16_t *bufPtr = buf;

    for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
    {
        *bufPtr++ = *pixelPtr++;
    }

    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////

    // tileToBuf(buf, (TileRef) tileAtLoc(x, y), all_layers);
    // Draw entity
    if(Entity *e = entityAtLocation(x, y))
    {
        drawToBuff(buf, e->type, 0, 0);
    }
    // Draw tile overlay
    // tileToBuf(buf, (TileRef) tileAtLoc(x, y), overlay_layer);

    // Draw entity overlay (from tile in front)
    if(Entity *e = entityAtLocation(x, y+1))
    {
        drawToBuff(buf, e->type, 0, 1);
    }

    screen.drawCellBuffer(buf, x, y);
}

/////////////////////////////////////////////////////


int tileAtLoc(int x, int y)
{
    return environmentList[envId].terrain[y * GRID_WIDTH + x];
}

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

void updateCrate(Entity *crate)
{
    // Numbers are tile indexes from 'tile_ref_01.png'
    if(tileAtLoc(crate->x, crate->y) == 9 || tileAtLoc(crate->x, crate->y) == 12)
        crate->type = crate_active_t;
    else
        crate->type = crate_t;
}

bool gameSolved()
{
    // Test if every goal has a crate on the same location.
    for(int y = 0; y < GRID_HEIGHT; y++)
    {
        for(int x = 0; x < GRID_WIDTH; x++)
        {
            // Numbers are tile indexes from 'tile_ref_01.png'
            if(tileAtLoc(x, y) == 9 || tileAtLoc(x, y) == 12)
            {
                Entity *e = entityAtLocation(x, y);
                if(e == NULL || e->type != crate_active_t)
                    return false;
            }
        }
    }
    return true;
}


struct Entity *entityAtLocation(int x, int y)
{
    for(int i = 0; i < currentEntityLength; i++)
    {
        if(currentEntities[i].x == x && currentEntities[i].y == y)
            return &currentEntities[i];
    }
    return NULL;
}


// *IMPORTANT* The order of these must match TileRef enum for indexed lookup
const TileSpec tileLUT[] = {
    //{ <TileRef> , <base material> , <overlay material> , <blocks_motion> }
    // Bases
    {missing_tr, null_t, null_t, false},
    {floor_tr, floor_t, null_t, false},
    {stone_tr, stone_t, null_t, true},
    {water_tr, water_t, null_t, true},
    // Features
    {stone_front_tr, stone_front_t, null_t, true},
    {stone_w_tr, stone_w_t, null_t, true},
    {stone_e_tr, stone_e_t, null_t, true},
    {stone_nw_tr, stone_nw_t, null_t, true},
    {stone_ne_tr, stone_ne_t, null_t, true},
    {stone_sw_tr, stone_sw_t, null_t, true},
    {stone_se_tr, stone_se_t, null_t, true},
    // Compound 
    {floor_stone_overhang_tr, floor_t, stone_overhang_t, false},
    {floor_target_tr, floor_t, goal_t, false},
    {water_stone_overhang_tr, water_t, stone_overhang_t, true},
    {water_target_tr, water_t, goal_t, true},
};

// Indexed against tile_ref_01.png
const bool tile_blocks_motion[] = {
    true,
    true,
    true,
    true,
    true,
    true,
    false, //floor
    true,
    true,
    false, //goal + overhang
    false, // wall_cnr_cv_sw
    true,
    false, //goal
    true,
    true,
    true,
    true,
    false, // wall_s
    true,
};

bool terrainBlocksMovement(int x, int y)
{
    TileRef tile = (TileRef) tileAtLoc(x,y);
    return tile_blocks_motion[tile];
    // return tileLUT[tile].blocks_motion;
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


void spriteToBuf(uint16_t *buf, int x, int y)
{
    int offset =  y * GRID_SIZE * SPRITESHEET_WIDTH + x * GRID_SIZE;
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

void tileToBuf(uint16_t *buf, TileRef tile, TileLayer layer)
{
    if(layer == base_layer)
    {
        drawToBuff(buf, tileLUT[tile].base, 0, 0);
    }
    else if (layer == overlay_layer && tileLUT[tile].overlay != null_t)
    {
        drawToBuff(buf, tileLUT[tile].overlay, 0, 0);
    }
    else if (layer == all_layers)
    {
        drawToBuff(buf, tileLUT[tile].base, 0, 0);
        if(tileLUT[tile].overlay != null_t)
            drawToBuff(buf, tileLUT[tile].overlay, 0, 0);
    }
}

void drawToBuff(uint16_t *buf, MaterialType type, int offsetX, int offsetY)
{
    switch(type)
    {
        case null_t:
            if(offsetX != 0 || offsetY != 0)
                return;
                break;
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
        case stone_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_X - offsetX;
            offsetY = STONE_Y - offsetY;
            break;

        case stone_e_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_E_X - offsetX;
            offsetY = STONE_E_Y - offsetY;
                break;
            
        case stone_w_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_W_X - offsetX;
            offsetY = STONE_W_Y - offsetY;
                break;
            
        case stone_nw_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_NW_X - offsetX;
            offsetY = STONE_NW_Y - offsetY;
                break;
            
        case stone_sw_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_SW_X - offsetX;
            offsetY = STONE_SW_Y - offsetY;
                break;
            
        case stone_se_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_SE_X - offsetX;
            offsetY = STONE_SE_Y - offsetY;
                break;
            
        case stone_ne_t:
            if(offsetX != 0 || offsetY != 0)
                return;
            offsetX = STONE_NE_X - offsetX;
            offsetY = STONE_NE_Y - offsetY;
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
            offsetX = STONE_X - offsetX;
            offsetY = STONE_Y - offsetY;
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
    spriteToBuf(buf, offsetX, offsetY);
}

void screenSetup()
{
    screen.begin();
    screen.setRotation(1);
    screen.drawIntro();
}
void screenDrawBuf(uint16_t *buf, int x, int y){screen.drawCellBuffer(buf, x, y);}
void screenSuccess(){ screen.drawSuccess(); }
void screenEnvComplete(){ screen.drawMapComplete(); }

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