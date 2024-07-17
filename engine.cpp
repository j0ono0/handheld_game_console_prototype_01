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

    uint16_t buf[512];
    Entity *plr = currentEntities;

    // Draw entities (just plr for now)
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].type == plr_t)
            plr = &currentEntities[i];
            blitTerrain(plr->x, plr->y-2, 2, 4, buf);
            blitPlr(0, 0, 2, 4, buf, &plr_sprite[0]); 
            screen.writeRect(plr->x*GRID_SIZE, (plr->y-2)*GRID_SIZE, 2*GRID_SIZE, 4*GRID_SIZE, buf);
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

            const uint16_t *spritePtr = &sprite_8x8_overlays[GRID_SIZE * GRID_SIZE * (overlayId)];

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

void blitPlr(int x, int y, int w, int h, uint16_t *buf, const uint16_t *spriteSrc)
{
    // x, y are in PIXEL units <------- IMPORTANT! ---- 
    // w, h in tiles
    // w and h are dimensions of buf

    uint16_t *bufPtr = &buf[y * w * GRID_SIZE + x];
    
    // copy sprite into buf. 
    //Plr is 2x4 tiles (512 pixels) big so row = 2 and col = 4
    for(int row = 0; row < 4 * GRID_SIZE ; ++row)
    {
        for(int col = 0; col < 2 * GRID_SIZE ; ++col)
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



void walkPlr_animated(Entity *plr, int x, int y)
{
    // tiles used areeither 4x4 (horizontal move) or 2x6 (vertical move)
    // Use larger of the two.
    uint16_t buf[4096];

    int buf_w = 2;
    int buf_h = 4;
    int terrain_x = plr->x;
    int terrain_y = (plr->y - 2); // offset to allow sprite overlay
    // sprite locations are relative to terrain
    int start_x = 0;
    int start_y = 0;
    int end_x = 0;
    int end_y = 0;

    // update plr location,
    plr->x += x * 2;
    plr->y += y * 2;




    // buffer shape and screen-location depend on moving up/down or left/right
    // NOTE: all movements are x2 tile regardless of direction!
    if(x < 0){
        // move west
        buf_w = 4;
        terrain_x -= 2;
        start_x = 2 * GRID_SIZE;
    }
    else if(x > 0){
        // move east
        buf_w = 4;
        end_x = 2 * GRID_SIZE;
    }
    else if(y < 0){
        // move north
        buf_h = 6;
        terrain_y -= 2;
        start_y = 2 * GRID_SIZE;
    }
    else if (y > 0){
        // move south
        buf_h = 6;
        end_y = 2 * GRID_SIZE;
    }
    
    for(int i = 0; i < 4; ++i)
    {
        static int sOffset = 512;
        sOffset = sOffset >= 3*512 ? 512 : sOffset + 512;

        blitTerrain(terrain_x, terrain_y, buf_w, buf_h, buf); 
        blitPlr(start_x, start_y, buf_w, buf_h, buf, &plr_sprite[sOffset]);  
        blitOverlay(terrain_x, terrain_y, buf_w, buf_h, buf); 

        screen.writeRect(terrain_x*GRID_SIZE, terrain_y*GRID_SIZE, buf_w*GRID_SIZE, buf_h*GRID_SIZE, buf);

        delay(100);

        start_x += 4 * x;
        start_y += 4 * y;

    }

    blitTerrain(terrain_x, terrain_y, buf_w, buf_h, buf); 
    blitPlr(end_x, end_y, buf_w, buf_h, buf, &plr_sprite[0]);  
    blitOverlay(terrain_x, terrain_y, buf_w, buf_h, buf); 

    screen.writeRect(terrain_x*GRID_SIZE, terrain_y*GRID_SIZE, buf_w*GRID_SIZE, buf_h*GRID_SIZE, buf);


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