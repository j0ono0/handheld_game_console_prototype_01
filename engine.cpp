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
    drawTerrain(0, 0, GRID_WIDTH, GRID_HEIGHT);

    // Draw entities (just plr for now)
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].type == plr_t)
            drawPlr(currentEntities[i].x, currentEntities[i].y);
    }
}



void blitTerrain(int x, int y, int w, int h, uint16_t *buf)
{
    uint16_t *cellbuf = &buf[0];

    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            
            uint16_t tileId = environmentList[envId].terrain[(y + i) * GRID_WIDTH + (x + j)];
            const uint16_t *spritePtr = &sprite_tile_ref_8x8[GRID_SIZE * GRID_SIZE * tileId];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i*w*GRID_SIZE*GRID_SIZE + j*GRID_SIZE];

            for(int row = 0; row < GRID_SIZE; ++row)
            {
                for(int col = 0; col < GRID_SIZE; ++col)
                {
                    // Transfer row to buf
                    *cellbuf++ = *spritePtr++;
                }
                // Move cellbuf to start of next line
                cellbuf += w*GRID_SIZE - GRID_SIZE;
            }
        }
    }
}

void drawTerrain(int x, int y, int w, int h)
{
    uint16_t buf[w * GRID_SIZE * h * GRID_SIZE];
    blitTerrain(x, y, w, h, buf);
    screen.writeRect(x * GRID_SIZE, y * GRID_SIZE, w * GRID_SIZE , h * GRID_SIZE , buf);
}

void drawPlr(int x, int y)
{
    // Offset to position plr sprite at correct terrain location
    y -= 3;
    // plr sprite takes up x8 grid cells 
    // 16px wide, 32px high, 512pixels, starting at offset of 0.

    uint16_t buf[3 * 4 * GRID_SIZE * GRID_SIZE];
    blitTerrain(x, y, 2, 4, buf);
    blitPlr(0, 0, 3, 4, buf, plr_sprite);
    screen.writeRect(x * GRID_SIZE, y * GRID_SIZE, 16, 32, buf);
}


void blitPlr(int x, int y, int bufW, int bufH, uint16_t *buf, const uint16_t *spriteSrc)
{
    // *** NOTE: w and h are dimensions of buf ***
    bufW *= GRID_SIZE;
    bufH *= GRID_SIZE;

    //********** */
    // NOTE: x and y are **PIXEL** OFFSET in buf
    //********** */

    uint16_t *bufPtr = &buf[y*bufW + x];
    
    // copy sprite into buf. Plr is 2x4 tiles (512 pixels) big
    for(int row = 0; row < 4*GRID_SIZE; ++row)
    {
        for(int col = 0; col < 2*GRID_SIZE; ++col)
        {
            // Transfer row to buf
            if(*spriteSrc != COLOR_TRANSPARENT)
                *bufPtr = *spriteSrc;
            ++bufPtr;
            ++spriteSrc;
        }
        bufPtr += bufW - 2*GRID_SIZE;
    }
}

void walkPlr(Entity *plr, int x, int y)
{
    // buffer covers plr sprite and possible movement locations in x and y directions.
    int bufW = 3;
    int bufH = 5;
    uint16_t buf[bufW*GRID_SIZE * bufH*GRID_SIZE];

    // Buffer placement in terrain (tile values)
    int tx = plr->x;
    int ty = plr->y;
    
    // Sprite offset in buf (px values)
    int sx = 0;
    int sy = 0;

    if(x < 0)
    {
        tx += x;
        sx = GRID_SIZE;
    }
    if (y < 0){
        ty += y;
        sy = GRID_SIZE;
    }
  
    int sprite_offset = 0;

    for(int i = 0; i < GRID_SIZE; ++i)
    {
        sx += x;
        sy += y;

        sprite_offset += 512;
        if(sprite_offset >=2048)
            sprite_offset = 512;

        blitTerrain(tx, ty - 3, bufW, bufH, buf);
        blitPlr(sx, sy, bufW, bufH, buf, &plr_sprite[sprite_offset]);
        screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);
        delay(60);
    }

    // Finish in standing position
    blitTerrain(tx, ty - 3, bufW, bufH, buf);
    blitPlr(sx, sy, bufW, bufH, buf, &plr_sprite[0]);
    screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);

    // update plr location
    plr->x += x;
    plr->y += y;  

    
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
// const TileSpec tileLUT[] = {
//     //{ <TileRef> , <base material> , <overlay material> , <blocks_motion> }
//     // Bases
//     {missing_tr, null_t, null_t, false},
//     {floor_tr, floor_t, null_t, false},
//     {stone_tr, stone_t, null_t, true},
//     {water_tr, water_t, null_t, true},
//     // Features
//     {stone_front_tr, stone_front_t, null_t, true},
//     {stone_w_tr, stone_w_t, null_t, true},
//     {stone_e_tr, stone_e_t, null_t, true},
//     {stone_nw_tr, stone_nw_t, null_t, true},
//     {stone_ne_tr, stone_ne_t, null_t, true},
//     {stone_sw_tr, stone_sw_t, null_t, true},
//     {stone_se_tr, stone_se_t, null_t, true},
//     // Compound 
//     {floor_stone_overhang_tr, floor_t, stone_overhang_t, false},
//     {floor_target_tr, floor_t, goal_t, false},
//     {water_stone_overhang_tr, water_t, stone_overhang_t, true},
//     {water_target_tr, water_t, goal_t, true},
// };

// Indexed against tile_ref_01.png
const bool tile_blocks_motion[] = {
    false,  // void
    true,
    true,
    true,
    true,
    true,
    true, 
    true,
    true,
    true, 
    true, 
    false,  // floor
    true,   // wall_cnr_nw
    true,   // wall_cnr wsw
    false,  // wall_cnr_sw
    false,  // wall_s
    false,  // wall_cnr_sw
    true,   // wall_cnr_ne
    true,
    false,  // wall_cnr_ssw
    true,   // wall_cnr_sw
};


bool terrainBlocksMovement(int x, int y, int w, int h)
{
    // Width and height are size of entity
    // Note: tile map is 8x8 whilst entities are normally 16x16 in size.
    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            if(tile_blocks_motion[(TileRef) tileAtLoc(x+i,y+j)])
                return true;
        }
    }
    return false;
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