#include "engine.h"

Extended_Tft screen = Extended_Tft(TFT_CS, TFT_DC);

// Current environment
int envId = 0;

// Entities currently in game.
Entity currentEntities[MAX_ENTITIES];
int currentEntityLength = 0;


const TileSpec tile_attr[] = {
    {void_tn, null_to, true},
    {floor_tn, null_to, false},
    {stone_tn, null_to, true},
    {wall_cv_ne_tn, null_to, true},
    {wall_n_tn, null_to, true},
    {wall_cv_nw_tn, null_to, true},
    {stoneedge_cv_se_tn, stoneedge_cv_se_to, false},
    {stoneedge_s_tn, stone_edge_s_to, false},
    {stoneedge_sw_tn, stoneedge_cv_sw_to, false},
    {stoneedge_e_tn, null_to, true},
    {stoneedge_w_tn, null_to, true},
    {stoneedge_cv_ne_tn, null_to, true},
    {stoneedge_n_tn, null_to, true},
    {stoneedge_cv_nw_tn, null_to, true},
    {stoneedge_cc_wnw_tn, null_to, true},
    {stoneedge_cc_nw_tn, null_to, true},
    {stoneedge_cc_ne_tn, null_to, true},
    {stoneedge_cc_ene_tn, null_to, true},
    {stoneedge_cc_wsw_tn, null_to, true},
    {stoneedge_cc_sw_tn, stoneedge_cc_sw_to, false},
    {stoneedge_cc_se_tn, stoneedge_cc_se_to, false},
    {stoneedge_cc_ese_tn, null_to, true},
};

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
    drawTerrain(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT);

    // Draw entities (just plr for now)
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].type == plr_t)
            drawPlr(currentEntities[i].x, currentEntities[i].y);
    }
}


void blitOverlay(int x, int y, int w, int h, uint16_t *buf)
{
    // x, y, w, and h are in multiples of tiles (grid_size)
    uint16_t *cellbuf = buf;

    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            uint16_t tileId = environmentList[envId].terrain[(y + i) * TERRAIN_WIDTH + (x + j)];
            int overlayId = tile_attr[tileId].overlay;

            // Skip tiles with no overlay
            if( overlayId == null_to)
                continue;

            // overlayId decrimented by -1 as no null_to sprite exists - sprites are offset by 1 tile
            // compared to TileOverlay enum.

            const uint16_t *spritePtr = &sprite_8x8_overlays[GRID_SIZE * GRID_SIZE * (overlayId-1)];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i*w*GRID_SIZE*GRID_SIZE + j*GRID_SIZE];

            for(int row = 0; row < GRID_SIZE; ++row)
            {
                for(int col = 0; col < GRID_SIZE; ++col)
                {
                    if(*spritePtr != COLOR_TRANSPARENT)
                    {
                    // Transfer row to buf
                    *cellbuf = *spritePtr;
                    }
                    ++cellbuf;
                    ++spritePtr;
                }
                // Move cellbuf to start of next line
                cellbuf += w*GRID_SIZE - GRID_SIZE;
            }
        }
    }
}

void blitTerrain(int x, int y, int w, int h, uint16_t *buf)
{
    // x, y, w, and h are in multiples of tiles (grid_size)
    uint16_t *cellbuf = buf;

    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            uint16_t tileId = environmentList[envId].terrain[(y + i) * TERRAIN_WIDTH + (x + j)];
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
    y -= 2;
    // plr sprite takes up x8 grid cells 
    // 16px wide, 32px high, 512pixels, starting at offset of 0.

    uint16_t buf[2 * 4 * GRID_SIZE * GRID_SIZE];
    blitTerrain(x, y, 2, 4, buf);
    blitPlr(2, 4, buf, plr_sprite);


    screen.writeRect(x * GRID_SIZE, y * GRID_SIZE, 16, 32, buf);
}


void blitPlr(int w, int h, uint16_t *buf, const uint16_t *spriteSrc)
{
    // *** NOTE: w and h are dimensions of buf ***

    uint16_t *bufPtr = buf;
    
    // copy sprite into buf. Plr is 2x4 tiles (512 pixels) big
    for(int row = 0; row < h * GRID_SIZE; ++row)
    {
        for(int col = 0; col < w * GRID_SIZE; ++col)
        {
            // Transfer row to buf
            if(*spriteSrc != COLOR_TRANSPARENT)
                *bufPtr = *spriteSrc;
            ++bufPtr;
            ++spriteSrc;
        }
        // Move to start of next row
        bufPtr += w * GRID_SIZE - 2 * GRID_SIZE;
    }
}

void walkPlr_doublestep(Entity *plr, int x, int y)
{
    // buffer is 2x4 tiles in size
    uint16_t buf[512];
    // delete old plr sprite
    blitTerrain(plr->x, plr->y-2, 2, 4, buf);
    screen.writeRect(plr->x*GRID_SIZE, (plr->y-2)*GRID_SIZE, 2*GRID_SIZE, 4*GRID_SIZE, buf);

    // update plr location
    plr->x += x * 2;
    plr->y += y * 2; 

    blitTerrain(plr->x, plr->y-2, 2, 4, buf);
    blitPlr(2, 4, buf, &plr_sprite[0]);    

    // Draw overlaying terrain
    blitOverlay(plr->x, plr->y-2, 2, 4, buf);
    
    screen.writeRect(plr->x*GRID_SIZE, (plr->y-2)*GRID_SIZE, 2*GRID_SIZE, 4*GRID_SIZE, buf);


    // // buffer covers plr sprite and possible movement locations in x and y directions.
    // int bufW = 4;
    // int bufH = 6;
    // uint16_t buf[bufW*GRID_SIZE * bufH*GRID_SIZE];

    // // Buffer placement in terrain (tile values)
    // // tx, and ty are tile locations
    // int tx = plr->x;
    // int ty = plr->y;
    
    // // Sprite offset in buf (px values)
    // int sx = 0;
    // int sy = 0;

    // if(x < 0)
    // {
    //     tx += x * 2;
    //     sx = GRID_SIZE * 2;
    // }
    // if (y < 0){
    //     ty += y * 2;
    //     sy = GRID_SIZE * 2;
    // }
  
    // blitTerrain(tx, ty - 3, bufW, bufH, buf);
    // blitPlr(sx, sy, bufW, bufH, buf, &plr_sprite[0]);
    // screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);


    // int sprite_offset = 0;
    // for(int i = 0; i < GRID_SIZE; ++i)
    // {
    //     sx += x;
    //     sy += y;

    //     sprite_offset += 512;
    //     if(sprite_offset >=2048)
    //         sprite_offset = 512;

    //     blitTerrain(tx, ty - 3, bufW, bufH, buf);
    //     blitPlr(sx, sy, bufW, bufH, buf, &plr_sprite[sprite_offset]);
    //     screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);
    //     delay(40);
    // }

    // // Finish in standing position
    // blitTerrain(tx, ty - 3, bufW, bufH, buf);
    // blitPlr(sx, sy, bufW, bufH, buf, &plr_sprite[0]);
    // screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);

 
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
        blitPlr(bufW, bufH, buf, &plr_sprite[sprite_offset]);
        screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);
        delay(40);
    }

    // Finish in standing position
    blitTerrain(tx, ty - 3, bufW, bufH, buf);
    blitPlr(bufW, bufH, buf, &plr_sprite[0]);
    screen.writeRect(tx*GRID_SIZE, (ty-3)*GRID_SIZE, bufW*GRID_SIZE, bufH*GRID_SIZE, buf);

    // update plr location
    plr->x += x;
    plr->y += y;  
}

/////////////////////////////////////////////////////


int tileAtLoc(int x, int y)
{
    return environmentList[envId].terrain[y * TERRAIN_WIDTH + x];
}

bool inbounds(int x, int y)
{
    if (x < 0 || y < 0 || x > TERRAIN_WIDTH || y > TERRAIN_HEIGHT)
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
    for(int y = 0; y < TERRAIN_HEIGHT; y++)
    {
        for(int x = 0; x < TERRAIN_WIDTH; x++)
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




bool terrainBlocksMovement(int x, int y, int w, int h)
{
    // Width and height are size of entity
    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            if(tile_attr[tileAtLoc(x+i,y+j)].blocks_motion)
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