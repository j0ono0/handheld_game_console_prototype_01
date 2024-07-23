#include "engine.h"

Extended_Tft screen = Extended_Tft(TFT_CS, TFT_DC);

uint16_t screenbuf[TERRAIN_HEIGHT * TERRAIN_UNIT * TERRAIN_WIDTH * TERRAIN_UNIT];


// throttle animation cycles
unsigned long ani_clock = 0;
// flag if motion animation is currently happening
bool _sprites_in_motion = false;

// Current environment
int envId = 0;

// Entities currently in environment.
Entity currentEntities[MAX_ENTITIES];
Entity *entitiesInDrawOrder[MAX_ENTITIES];
int currentEntityLength = 0;

const TileSpec tile_layers[] = {
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

// Sprite locations 
// IMPORTANT: order must match enum EntityType
EntitySpecs sprite_specs[] = {
    {plr_t, {16, 32}, &entity_sprites[0]},   // Plr standing
    {crate_t, {16, 32}, &entity_sprites[8*ENV_UNIT*ENV_UNIT]},  // Crate
    {crate_active_t, {16, 32}, &entity_sprites[10*ENV_UNIT*ENV_UNIT]},  // Crate active
    {target_t, {16, 16}, &entity_sprites[12*ENV_UNIT*ENV_UNIT]},  // Target
};

const uint16_t *sprite_plr_stationary = &entity_sprites[0];
const uint16_t *sprites_plr_walking[] = {
    &entity_sprites[1*ENV_UNIT*2*ENV_UNIT],
    &entity_sprites[1*ENV_UNIT*2*ENV_UNIT*2],
    &entity_sprites[1*ENV_UNIT*2*ENV_UNIT*3],
    &entity_sprites[1*ENV_UNIT*2*ENV_UNIT*2],
};


/////////////////////////////////////////////////////

void populateCurrentEntities()
{
    currentEntityLength = 0;
    for(int i = 0; i < environmentList[envId].entity_count; ++i)
    {
        currentEntities[currentEntityLength] = environmentList[envId].entities[i];
        entitiesInDrawOrder[currentEntityLength] = &currentEntities[currentEntityLength];

        currentEntityLength++;
    }
    sortEntityDrawOrder();
}

Entity *assignPlayer()
{
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].type == plr_t)
            currentEntities[i].behaviour = &act_test;
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


void drawAll()
{
    blitTerrain(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT, screenbuf);
    drawEntities();
    blitOverlay(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT, screenbuf);
    screen.writeRect(0, 0, TERRAIN_WIDTH * TERRAIN_UNIT ,TERRAIN_HEIGHT * TERRAIN_UNIT ,screenbuf);
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
            int overlayId = tile_layers[tileId].overlay;

            // Skip tiles with no overlay
            if( overlayId == null_to)
                continue;

            const uint16_t *spritePtr = &sprite_8x8_overlays[TERRAIN_UNIT * TERRAIN_UNIT * (overlayId)];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i*w*TERRAIN_UNIT*TERRAIN_UNIT + j*TERRAIN_UNIT];

            for(int row = 0; row < TERRAIN_UNIT; ++row)
            {
                for(int col = 0; col < TERRAIN_UNIT; ++col)
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
                cellbuf += w*TERRAIN_UNIT - TERRAIN_UNIT;
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
            const uint16_t *spritePtr = &sprite_tile_ref_8x8[TERRAIN_UNIT * TERRAIN_UNIT * tileId];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i*w*TERRAIN_UNIT*TERRAIN_UNIT + j*TERRAIN_UNIT];

            for(int row = 0; row < TERRAIN_UNIT; ++row)
            {
                for(int col = 0; col < TERRAIN_UNIT; ++col)
                {
                    // Transfer row to buf
                    *cellbuf++ = *spritePtr++;
                }
                // Move cellbuf to start of next line
                cellbuf += w*TERRAIN_UNIT - TERRAIN_UNIT;
            }
        }
    }
}

bool renderBefore(Entity *a, Entity *b)
{
    if(a->type == target_t){
        return true;
    }
    if(a->y < b->y && b->type != target_t)
    {
        return true;
    }
    return false;
}

void sortEntityDrawOrder(){
    // Sort entitiesInDrawOrder array
    // Insertion sort
    int i, j;
    Entity *key;
    for (i = 1; i < currentEntityLength; i++) {
        key = entitiesInDrawOrder[i];
        j = i - 1;

        // while (j >= 0 && entitiesInDrawOrder[j]->y > key->y) {
        while (j >= 0 && renderBefore(key, entitiesInDrawOrder[j])) {
            entitiesInDrawOrder[j + 1] = entitiesInDrawOrder[j];
            j = j - 1;
        }
        entitiesInDrawOrder[j + 1] = key;
    }
}

void drawEntities()
{
    // draw entities into buffer
    for(int i = 0; i < currentEntityLength; ++i)
    {
        blitEntity(entitiesInDrawOrder[i], screenbuf);
    }

}

void blitEntity(Entity *e, uint16_t *buf)
{

    int x = e->x * ENV_UNIT - e->mx;
    int y = e->y * ENV_UNIT - e->my + (ENV_UNIT - sprite_specs[e->type].dimensions.h);

    const uint16_t *sprite_ptr = sprite_specs[e->type].sprite_addr;
    uint16_t *bufPtr = &buf[y * SCREEN_WIDTH + x];
    
    // copy sprite into buf. 
    //Plr is 2x4 tiles (512 pixels) big so row = 2 and col = 4
    for(int row = 0; row < sprite_specs[e->type].dimensions.h ; ++row)
    {
        for(int col = 0; col < sprite_specs[e->type].dimensions.w ; ++col)
        {
            // Transfer row to buf
            if(*sprite_ptr != COLOR_TRANSPARENT)
                *bufPtr = *sprite_ptr;
            ++bufPtr;
            ++sprite_ptr;
        }
        // Move to start of next row
        bufPtr += SCREEN_WIDTH - ENV_UNIT;
    }
}

void moveEntity(Entity *e, int dx, int dy)
{
    // Block function until sprite has caught up?
    // TODO: make an event queue instead? 
    // if(e->mx != 0 || e->my != 0)
    //     return;

    // Update entity coordinates
    e->x += dx;
    e->y += dy;
    // mx and my are in pixels.
    // they indicate direction and distance sprite has to move
    e->mx += dx * ENV_UNIT;
    e->my += dy * ENV_UNIT;

    // Draw order may need rearranging if entity moved vertically
    if(dy !=0)
        sortEntityDrawOrder();
}

/////////////////////////////////////////////////////
// Sprite controls                                ///

void updateSprites()
{
    // Progress all animation cycles with 'step'.
    #define MAXANIMATIONSTEPS 4
    static int step = 0;

    int now = millis();
    if(now - ani_clock >= 80)
    {
        _sprites_in_motion = false;

        step = step + 1 < MAXANIMATIONSTEPS ? step + 1 : 0;
        ani_clock = now;
        for(int i = 0; i < currentEntityLength; ++i)
        {
            Entity *e = &currentEntities[i];
            int STEP_DISTANCE = 4; 
            if(e->mx ==0 && e->my == 0)
            {
                if(e->type == plr_t){
                    sprite_specs[plr_t].sprite_addr = sprite_plr_stationary;
                }
            }
            else
            {
                _sprites_in_motion = true;
                    
                if(e->type == plr_t){
                    sprite_specs[plr_t].sprite_addr = sprites_plr_walking[step];
                }

                
                if(e->mx > 0){
                    e->mx -= STEP_DISTANCE;
                }else if(e->mx < 0){
                    e->mx += STEP_DISTANCE;
                }
                if(e->my > 0){
                    e->my -= STEP_DISTANCE;
                }else if(e->my < 0){
                    e->my += STEP_DISTANCE;
                }

            }
           
        }
    }
}

bool spritesInMotion()
{
    return _sprites_in_motion;
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



bool gameSolved()
{
    Entity *target;
    Entity *crate;
    // Test if every target has a crate on the same location.
    for(int i = 0; i < currentEntityLength; i++)
    {
        target = &currentEntities[i];
        if(target->type == target_t)
        {
            bool has_crate = false;
            
            for(int j = 0; j < currentEntityLength; j++)
            {
                crate = &currentEntities[j];
                if((crate->type == crate_t || crate->type == crate_active_t)){

                    if(coLocated(target, crate))
                    {
                        has_crate = true;
                    }

                }
            }
            if(!has_crate) 
            {
            return false;
            }
        }
    }
    Serial.println("game solved!");
    return true;
}


struct Entity *entityAtLocation(int x, int y)
{
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].x == x && currentEntities[i].y == y && currentEntities[i].type != target_t)
            return &currentEntities[i];
    }
    return NULL;
}




bool terrainBlocksMovement(int x, int y, int w, int h)
{
    // x and y are ENV_UNITS, convert to TERRAIN_UNITS
    int tx = x * ENV_UNIT / TERRAIN_UNIT;
    int ty = y * ENV_UNIT / TERRAIN_UNIT;
    // Width and height are size of entity footprint
    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            if(tile_layers[tileAtLoc(tx+i,ty+j)].blocks_motion)
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


void act_test()
{
    Serial.println("hello entity animation action.");
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