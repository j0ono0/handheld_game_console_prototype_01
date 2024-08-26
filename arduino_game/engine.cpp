#include "engine.h"

#include "resources.h"

//////////////////////////////////////////////////////////////////
/// External graphics and data                                  //

extern const uint16_t prof_walk_01[];
extern const uint16_t entity_sprites_2[];

extern const uint8_t terrain_tiles_indexed[];
extern const uint16_t terrain_color_table[];


// extern const EnvSpec environmentList[];

// extern const Sprite sprite_target;
// extern const Sprite sprite_crate;
// extern const Sprite sprite_crate_active;
// extern const Sprite sprite_powerconverter;
// extern const Sprite sprite_powerconverter_active;

// extern const Sprite sprite_prof_stationary_left;
// extern const Sprite sprite_prof_stationary_right;

// extern Sprite *prof_walk_east_cycle[];
// extern Sprite *prof_walk_west_cycle[];
// extern Sprite *prof_walk_north_cycle[];
// extern Sprite *prof_walk_south_cycle[];
/////////////////////////////////////////////////////////////////

Extended_Tft screen = Extended_Tft(TFT_CS, TFT_DC);

uint16_t screenbuf[TERRAIN_HEIGHT * TERRAIN_UNIT * TERRAIN_WIDTH * TERRAIN_UNIT];


// throttle animation cycles
unsigned long ani_clock = 0;


GameManager gm = {gm_intro, 0, 0, {}};

// Entities currently in environment.
Entity *entitiesInDrawOrder[MAX_ENTITIES];



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


/////////////////////////////////////////////////////

void populateCurrentEntities()
{
    gm.e_len = 0;
    for(int i = 0; i < environmentList[gm.envId].entity_count; ++i)
    {
        gm.entities[gm.e_len] = environmentList[gm.envId].entities[i];

        gm.entities[gm.e_len] = environmentList[gm.envId].entities[i];
        entitiesInDrawOrder[gm.e_len] = &gm.entities[gm.e_len];

        gm.e_len++;
    }
    sortEntityDrawOrder();
}

Entity *assignPlayer()
{
    for(int i = 0; i < gm.e_len; ++i)
    {
        if(gm.entities[i].type == plr_t)
            return &gm.entities[i];
    }
    return NULL;
}

/////////////////////////////////////////////////////
// GameMode and Env controls /////////////////////////////////////

void setGameMode(GameMode mode)
{
    gm.mode = mode;
}
GameMode gameMode()
{
    return gm.mode;
}

int nextEnvironment()
{
    return setEnvironment(++gm.envId);
}
int setEnvironment(int envIndex)
{
    if(envIndex > 1)
    {
        gm.envId = 0;
    }
    gm.envId = envIndex;
    populateCurrentEntities();
    return gm.envId;
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
    blitTerrain(0, screenbuf);
    drawEntities(1);
    drawEntities(2);
    blitTerrain(3, screenbuf);
    drawEntities(4);
    screen.writeRect(0, 0, TERRAIN_WIDTH * TERRAIN_UNIT ,TERRAIN_HEIGHT * TERRAIN_UNIT ,screenbuf);
}


void blitTerrain(uint8_t layer, uint16_t *buf)
{
    // x, y, w, and h are in multiples of tiles (grid_size)
    uint16_t *cellbuf = buf;

    for(int i = 0; i < TERRAIN_HEIGHT; ++i)
    {
        for(int j = 0; j < TERRAIN_WIDTH; ++j)
        {
            uint8_t tileId = environmentList[gm.envId].terrain[i * TERRAIN_WIDTH +  j];

            if(tile_meta[tileId].layer != layer)
            {
                continue;
            }
                
            const uint8_t *spritePtr = &terrain_tiles_indexed[TERRAIN_UNIT * TERRAIN_UNIT * tileId];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i * TERRAIN_WIDTH * TERRAIN_UNIT * TERRAIN_UNIT + j * TERRAIN_UNIT];

            for(int row = 0; row < TERRAIN_UNIT; ++row)
            {
                for(int col = 0; col < TERRAIN_UNIT; ++col)
                {
                    // Transfer row to buf
                    *cellbuf = terrain_color_table[*spritePtr + gm.envId * 10];
                    ++cellbuf;
                    ++spritePtr;
                }
                // Move cellbuf to start of next line
                cellbuf += TERRAIN_WIDTH * TERRAIN_UNIT - TERRAIN_UNIT;
            }
        }
    }
}



// used in sortEntityDrawOrder
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
    for (i = 1; i < gm.e_len; i++) {
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
    for(int i = 0; i < gm.e_len; ++i)
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
    int y = e->y * ENV_UNIT - e->my + (ENV_UNIT - e->sprite->h);

    const uint16_t *sprite_ptr = e->sprite->addr;

    uint16_t *bufPtr = &buf[y * SCREEN_WIDTH + x];

    // copy sprite into buf. 
    for(int row = 0; row < e->sprite->h ; ++row)
    {
        for(int col = 0; col < e->sprite->w ; ++col)
        {
                // Transfer row to buf
                if(*sprite_ptr != COLOR_TRANSPARENT)
                    *bufPtr = *sprite_ptr;
                ++bufPtr;
                ++sprite_ptr; 
        }
        // Move screen buffer to next row
        bufPtr += SCREEN_WIDTH - e->sprite->w;
    }
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

    int now = millis();
    if(now - ani_clock < ANIMATIONSPEED) { return; }
    
    ani_clock = now;
    step = (step + 1) % MAXANIMATIONSTEPS;

    for(int i = 0; i < gm.e_len; ++i)
    {
        int8_t x_direction = 0;
        int8_t y_direction = 0;

        Entity *e = &gm.entities[i];
        if(e->mx != 0 || e->my != 0)
        {
            x_direction = (e->mx > 0) - (e->mx < 0); // results in +/- 1 or 0
            y_direction = (e->my > 0) - (e->my < 0); // results in +/- 1 or 0
            // Reduce distance to destination location
            e->mx -= STEP_DISTANCE * x_direction;
            e->my -= STEP_DISTANCE * y_direction;
        }

        switch(e->type)
        {
            case plr_t:
                if(spriteInTransit(e)) 
                {
                    if(x_direction > 0)
                    {
                        e->sprite = prof_walk_east_cycle[step%2];
                    }
                    else if (x_direction < 0)
                    {
                        e->sprite = prof_walk_west_cycle[step%2];
                    }
                    else if (y_direction < 0)
                    {
                        e->sprite = prof_walk_north_cycle[step%2];
                    }
                    else if (y_direction > 0)
                    {
                        e->sprite = prof_walk_south_cycle[step%2];
                    }
                }
                else
                {
                    if(x_direction > 0)
                    {
                        e->sprite = &sprite_prof_stationary_right;
                    }
                    else if (x_direction < 0)
                    {
                        e->sprite = &sprite_prof_stationary_left;
                    }else if (y_direction != 0)
                    {
                        e->sprite = &sprite_prof_stationary_left;
                    }
                }
                
                break;


            // case crate_t:
            //     if(entity_on_target(e))
            //     {
            //         e->sprite = &sprite_crate_active;
            //     }else{
            //         e->sprite = &sprite_crate;
            //     }
            //     break;
            case powerconverter_t:
            case powerconverter_active_t:
                if(entity_on_target(e, &gm))
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
    
}

bool spriteInTransit(Entity *e)
{
    return e->mx != 0 || e->my != 0;
}

bool spritesInTransit()
{
     for(int i = 0; i < gm.e_len; ++i)
    {
        if(spriteInTransit(&gm.entities[i]))
        { 
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////


int tileAtLoc(int x, int y)
{
    return environmentList[gm.envId].terrain[y * TERRAIN_WIDTH + x];
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

bool cheatGame(int keypress)
{
    // Hard coded BTN_SELECT for easy dev!
    if (keypress == 7)
    {
        return true;
    }
    return false;
}

bool gameSolved()
{
    Entity *target;
    Entity *crate;
    // Test if every target has a crate on the same location.
    for(int i = 0; i < gm.e_len; i++)
    {
        target = &gm.entities[i];
        if(target->type == target_t)
        {
            bool has_crate = false;
            
            for(int j = 0; j < gm.e_len; j++)
            {
                crate = &gm.entities[j];
                // TODO: This a is bad way to test if game is solved
                // make it better !!!
                if(
                    crate->type == crate_t || 
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





void screenSetup()
{   
    screen.begin();
    screen.setRotation(1);
    screenIntro();
}
void screenDrawBuf(uint16_t *buf, int x, int y){screen.drawCellBuffer(buf, x, y);}
void screenIntro(){ screen.drawIntro(); }
void screenSuccess(){ screen.drawSuccess(); }
void screenEnvComplete(){ screen.drawMapComplete(); }


/////////////////////////////////////////////////////////

void runBehaviours()
{
    for(int i = 0; i < gm.e_len; ++i)
    {
        if(gm.entities[i].behaviour)
        {
            (*gm.entities[i].behaviour)(&gm.entities[i], &gm);
        }
    }
}

bool entity_on_target(Entity *e, GameManager *gm)
{   
    for(int i = 0; i < gm->e_len; ++i)
    {
        if(gm->entities[i].type == target_t && coLocated(e, &gm->entities[i])){
            return true;
        }
    }
    return false;
}


