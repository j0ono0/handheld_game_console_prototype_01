#include "engine.h"



Extended_Tft screen = Extended_Tft(TFT_CS, TFT_DC);

uint16_t screenbuf[TERRAIN_HEIGHT * TERRAIN_UNIT * TERRAIN_WIDTH * TERRAIN_UNIT];

#define CSWIDTH 96 // width of 'entity_sprites_2' graphics resource

# define ENTITYSPEC(type, w, h, x, y) (EntitySpecs{type, {w, h}, &entity_sprites_2[96 * y + x]})

// throttle animation cycles
unsigned long ani_clock = 0;

// Current environment
uint8_t envId = 0;

// Entities currently in environment.
Entity currentEntities[MAX_ENTITIES];
Entity *entitiesInDrawOrder[MAX_ENTITIES];
int currentEntityLength = 0;



// This is auto-generated via python script
// Order matches tileset order
const TileMeta tile_meta[] =  {
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

// Sprite locations 
// IMPORTANT: order must match enum EntityType

// ***** character sprite sheet is 96 px wide ********

EntitySpecs sprite_specs[] = {
    ENTITYSPEC(plr_t, 16, 27, 0, 5),
    ENTITYSPEC(hoodie_t, 16, 30, 32, 2),
    ENTITYSPEC(strongman_t, 16, 32, 48, 0),
    ENTITYSPEC(officer_t, 16, 28, 65, 4),
    ENTITYSPEC(sunlover_t, 15, 28, 81, 4),
    ENTITYSPEC(office_chair_t, 15, 23, 16, 33),
    ENTITYSPEC(desktop_terminal_t, 15, 19, 17, 58),
    ENTITYSPEC(dotpanel_right_t, 13, 20, 0, 57),
    ENTITYSPEC(inbuilt_terminal_t, 14, 16, 32, 61),
    ENTITYSPEC(desk_clutter_t, 15, 18, 48, 59),
    ENTITYSPEC(target_t, 14, 13, 49, 43),
    ENTITYSPEC(crate_t, 16, 23, 64, 32),
    ENTITYSPEC(crate_active_t, 16, 23, 80, 32),
    ENTITYSPEC(powerconverter_t, 16, 25, 80, 55),
    ENTITYSPEC(powerconverter_active_t, 16, 25, 64, 55),



    {powerconverter_active_t, {16, 25}, &entity_sprites_2[96 * 55 + 64]},
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

        switch(currentEntities[currentEntityLength].type)
        {
            case plr_t:
                currentEntities[currentEntityLength].behaviour = &act_test;
                break;
            default:
                currentEntities[currentEntityLength].behaviour = &do_nothing;
                break;
        }
        currentEntityLength++;
    }
    sortEntityDrawOrder();
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

void advanceSpriteAnimations()
{
    // Progress all sprite movements of entities
        updateSprites();
        drawAll();
}

void drawAll()
{
    blitTerrain(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT, screenbuf);
    drawEntities(1);
    drawEntities(2);
    blitOverlay(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT, screenbuf);
    drawEntities(4);
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
            uint8_t tileId = environmentList[envId].terrain[(y + i) * TERRAIN_WIDTH + (x + j)];
            // Skip tiles on layer '0'
            if(tile_meta[tileId].layer == 0)
                continue;

            const uint8_t *spritePtr = &terrain_tiles_indexed[TERRAIN_UNIT * TERRAIN_UNIT * tileId];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i*w*TERRAIN_UNIT*TERRAIN_UNIT + j*TERRAIN_UNIT];

            for(int row = 0; row < TERRAIN_UNIT; ++row)
            {
                for(int col = 0; col < TERRAIN_UNIT; ++col)
                {
                    if(*spritePtr != COLOR_TRANSPARENT)
                    {
                    // Transfer row to buf
                    *cellbuf = terrain_color_table[*spritePtr + envId * 10];
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
            uint8_t tileId = environmentList[envId].terrain[(y + i) * TERRAIN_WIDTH + (x + j)];
            const uint8_t *spritePtr = &terrain_tiles_indexed[TERRAIN_UNIT * TERRAIN_UNIT * tileId];



            // Set cellbuf to start of tile section
            cellbuf = &buf[i*w*TERRAIN_UNIT*TERRAIN_UNIT + j*TERRAIN_UNIT];

            for(int row = 0; row < TERRAIN_UNIT; ++row)
            {
                for(int col = 0; col < TERRAIN_UNIT; ++col)
                {
                    // Transfer row to buf
                    *cellbuf++ = terrain_color_table[*spritePtr++ + envId * 10];
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

void drawEntities(int layer)
{
    // draw entities into buffer
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(entitiesInDrawOrder[i]->layer == layer)
        {
            blitEntity(entitiesInDrawOrder[i], screenbuf);
        }
    }

}

void blitEntity(Entity *e, uint16_t *buf)
{
    /* Copy sprite to buffer */

    /* TODO:
    
    walking plr to top of screen causes crash! sprite rendering outside buffer????
    
    */

    int x = e->x * ENV_UNIT - e->mx ;
    int y = e->y * ENV_UNIT - e->my + (ENV_UNIT - sprite_specs[e->type].dimensions.h);

    const uint16_t *sprite_ptr = sprite_specs[e->type].sprite_addr;
    uint16_t *bufPtr = &buf[y * SCREEN_WIDTH + x];

    int step_direction = 1;

    if(e->mx < 0)
    {
        // entity moving to left, flip sprite
        // TODO: record which sprites flip
        bufPtr += sprite_specs[e->type].dimensions.w;  
        step_direction = -1;
    }

    // copy sprite into buf. 
    for(int row = 0; row < sprite_specs[e->type].dimensions.h ; ++row)
    {
        for(int col = 0; col < sprite_specs[e->type].dimensions.w ; ++col)
        {
                // Transfer row to buf
                if(*sprite_ptr != COLOR_TRANSPARENT)
                    *bufPtr = *sprite_ptr;
                bufPtr += step_direction;
                ++sprite_ptr;
            
        }
        // Move to start of next row
        bufPtr += SCREEN_WIDTH - sprite_specs[e->type].dimensions.w * step_direction;
        sprite_ptr += CSWIDTH - sprite_specs[e->type].dimensions.w;
    }
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
    #define STEP_DISTANCE 4 
    #define ANIMATIONSPEED 80
    static int step = 0;
    // Track direction of change
    // used to update final entity (x, y) position
    int dx = 0;
    int dy = 0;

    int now = millis();
    if(now - ani_clock < ANIMATIONSPEED) { return; }
    
    ani_clock = now;
    step = (step + 1) % MAXANIMATIONSTEPS;

    for(int i = 0; i < currentEntityLength; ++i)
    {
        Entity *e = &currentEntities[i];
        if(e->mx == 0 && e->my == 0)
        {
            switch(e->type)
            {
                case plr_t:
                // sprite_specs[plr_t].sprite_addr = sprite_plr_stationary;
                break;
                case crate_t:
                case crate_active_t:
                    if(entity_on_target(e))
                    {
                        e->type = crate_active_t;
                    }else{
                        e->type = crate_t;
                    }
                    break;
                case powerconverter_t:
                case powerconverter_active_t:
                    if(entity_on_target(e))
                    {
                        e->type = powerconverter_active_t;
                    }else{
                        e->type = powerconverter_t;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            // // select plr walking sprite
            // if(e->type == plr_t){
            //     sprite_specs[plr_t].sprite_addr = sprites_plr_walking[step];
            // }
            //update x
            if(e->mx > 0){
                dx = 1;
            }else if(e->mx < 0){
                dx = -1;
            }
            // update y
            if(e->my > 0){
                dy = 1;
            }else if(e->my < 0){
                dy = -1;
            }
            // Reduce distance to destination location
            e->mx -= STEP_DISTANCE * dx;
            e->my -= STEP_DISTANCE * dy;
        }
    }
    
}

bool spritesInTransit()
{
     for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].mx != 0 || currentEntities[i].my != 0)
        { 
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////


int tileAtLoc(int x, int y)
{
    return environmentList[envId].terrain[y * TERRAIN_WIDTH + x];
}


bool inbounds(int x, int y)
{
    // Note: x and y in ENV_UNITS
    if (x < 0 || y < 0 || x >= ENV_WIDTH || y >= ENV_HEIGHT)
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
                // TODO: This a is bad way to test if game is solved
                // make it better !!!
                if(
                    crate->type == crate_t || 
                    crate->type == crate_active_t ||
                    crate->type == powerconverter_t ||
                    crate->type == powerconverter_active_t
                ){

                    if(coLocated(target, crate))
                    {
                        has_crate = true;
                    }

                }
            }
            if(!has_crate) return false;
        }
    }
    return true;
}


struct Entity *entityAtLocation(int x, int y, int layer)
{
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(
            currentEntities[i].x == x && 
            currentEntities[i].y == y &&
            currentEntities[i].layer == layer
        )
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
            if(tile_meta[tileAtLoc(tx+i,ty+j)].blocks_motion)
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

void runBehaviours()
{
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].behaviour)
        {
            (*currentEntities[i].behaviour)(&currentEntities[i]);
        }
    }
}

void do_nothing(Entity *e){}

void act_test(Entity *e)
{
    // Serial.println("hello entity animation action.");
}

bool entity_on_target(Entity *e)
{
    // Update crate appearance if on a target
    
    for(int i = 0; i < currentEntityLength; ++i)
    {
        if(currentEntities[i].type == target_t && coLocated(e, &currentEntities[i])){
            e->type = crate_active_t;
            return true;
        }
    }
    return false;
    e->type = crate_t;
}



