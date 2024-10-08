#include "engine.h"

// counts up to 200, incrementing every 80ms. 
// Totalling 16s maximum animation loop
// eg, The slowest a 2 frame animation will display is 8sec per frame 
// #define ANIMATIONSTEPS 200
#define ANIMATIONSPEED 80
#define SCREENROTATION 3


//////////////////////////////////////////////////////////////////
/// External graphics and data                                  //

extern const uint16_t prof_walk_01[];
extern const uint16_t entity_sprites_2[];

extern const uint8_t terrain_tiles_indexed[];
extern const uint16_t terrain_color_table[];


/////////////////////////////////////////////////////////////////
/// SPI devices                                                //


// set up variables using the SD utility library functions:
SdVolume volume;
SdFile root;
Sd2Card card;


XPT2046_Touchscreen  touchscreen(XPT_CS); // Touch screen user interface
Extended_Tft screen = Extended_Tft(TFT_CS, TFT_DC);



/////////////////////////////////////////////////////////////////

uint16_t screenbuf[TERRAIN_HEIGHT * TERRAIN_UNIT * TERRAIN_WIDTH * TERRAIN_UNIT];



GameManager gm = {0, {}, NULL, 0, 0};

// Entities currently in environment.
Entity *entitiesInDrawOrder[MAX_ENTITIES];

/////////////////////////////////////////////////////

void touchInit()
{
    touchscreen.begin();
    touchscreen.setRotation(SCREENROTATION); 
}

void sdCardTest()
{
    // Check for errors first
    screen.setTextColor(ILI9341_WHITE);
    screen.setTextSize(1);
    screen.setCursor(20, 20);

    if (!card.init(SPI_HALF_SPEED, SD_CS)) {
        screen.fillScreen(ILI9341_RED);
        screen.println("No SD card found.");
        delay(2000);
        return;
    }

    // Try to open the 'volume'/'partition' - it should be FAT16 or FAT32
    if (!volume.init(card)) {
        screen.fillScreen(ILI9341_RED);
        screen.println("No FAT16/FAT32 partition found.");
        screen.println("Is the card is formatted?");
        delay(2000);
        return;
    }
    ////////////////////////////////////////
    // print SD card details

    screen.fillScreen(0x0327);
    screen.print("\nCard type: ");
    switch(card.type()) {
    case SD_CARD_TYPE_SD1:
        screen.println("SD1");
        break;
    case SD_CARD_TYPE_SD2:
        screen.println("SD2");
        break;
    case SD_CARD_TYPE_SDHC:
        screen.println("SDHC");
        break;
    default:
        screen.println("Unknown");
    }

    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    screen.print("\nVolume type is FAT");
    screen.println(volume.fatType(), DEC);
    Serial.println();

    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    if (volumesize < 8388608ul) {
    screen.print("Volume size (bytes): ");
    screen.println(volumesize * 512);        // SD card blocks are always 512 bytes
    }
    screen.print("Volume size (Kbytes): ");
    volumesize /= 2;
    screen.println(volumesize);
    screen.print("Volume size (Mbytes): ");
    volumesize /= 1024;
    screen.println(volumesize);

    delay(2000);

}

void screenSetup()
{
    screen.begin();
    screen.setRotation(SCREENROTATION);
    screen.fillScreen(ILI9341_BLACK);
    
    // sdCardTest();
    // touchInit();

    screenIntro();
}

/////////////////////////////////////////////////////

void populateEntities(const Entity *entities, uint8_t length)
{
    gm.e_len = 0;
    for (int i = 0; i < length; ++i)
    {
        gm.entities[gm.e_len] = entities[i];

        gm.entities[gm.e_len] = entities[i];
        entitiesInDrawOrder[gm.e_len] = &gm.entities[gm.e_len];

        gm.e_len++;
    }
    sortEntityDrawOrder();
}

Entity *assignPlayer()
{
    for (int i = 0; i < gm.e_len; ++i)
    {
        if (gm.entities[i].type == plr_t)
            return &gm.entities[i];
    }
    return NULL;
}

void setTerrain(const uint8_t *terrain)
{
    gm.terrain = terrain;
}
void setPalette(const uint8_t palette)
{
    gm.palette = palette;
}

/////////////////////////////////////////////////////

void advanceSpriteAnimations()
{
    // Progress all sprite movements of entities
    if(advance_animation_clock(&gm.animation_clock))
    {
        updateSpriteTransits();
    }
    // TODO: Investigate syncing drawAll() to 
    // animation clock, it causes full screen to flicker (but should not?).
    drawAll();
    
}

void drawAll()
{
    blitTerrain(0, screenbuf);
    drawEntities(1);
    drawEntities(2);
    blitTerrain(3, screenbuf);
    drawEntities(4);
    screen.writeRect(0, 0, TERRAIN_WIDTH * TERRAIN_UNIT, TERRAIN_HEIGHT * TERRAIN_UNIT, screenbuf);
}

void blitTerrain(uint8_t layer, uint16_t *buf)
{
    // x, y, w, and h are in multiples of tiles (grid_size)
    uint16_t *cellbuf = buf;

    for (int i = 0; i < TERRAIN_HEIGHT; ++i)
    {
        for (int j = 0; j < TERRAIN_WIDTH; ++j)
        {
            uint8_t tileId = gm.terrain[i * TERRAIN_WIDTH + j];
            /////////////////////////////////////////////////////////////
            // tile_meta is in interactions.h -- not ideal filing?
            /////////////////////////////////////////////////////////////
            if (tile_meta[tileId].layer != layer)
            {
                continue;
            }

            const uint8_t *spritePtr = &terrain_tiles_indexed[TERRAIN_UNIT * TERRAIN_UNIT * tileId];

            // Set cellbuf to start of tile section
            cellbuf = &buf[i * TERRAIN_WIDTH * TERRAIN_UNIT * TERRAIN_UNIT + j * TERRAIN_UNIT];

            for (int row = 0; row < TERRAIN_UNIT; ++row)
            {
                for (int col = 0; col < TERRAIN_UNIT; ++col)
                {
                    // Transfer row to buf
                    // TODO: select indexed colour table row
                    // *cellbuf = terrain_color_table[*spritePtr + gm.envId * 10];
                    *cellbuf = terrain_color_table[*spritePtr + gm.palette];
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
    if (a->type == target_t)
    {
        return true;
    }
    if (a->y < b->y && b->type != target_t)
    {
        return true;
    }
    return false;
}

void sortEntityDrawOrder()
{
    // Sort entitiesInDrawOrder array
    // Insertion sort
    int i, j;
    Entity *key;
    for (i = 1; i < gm.e_len; i++)
    {
        key = entitiesInDrawOrder[i];
        j = i - 1;

        // while (j >= 0 && entitiesInDrawOrder[j]->y > key->y) {
        while (j >= 0 && renderBefore(key, entitiesInDrawOrder[j]))
        {
            entitiesInDrawOrder[j + 1] = entitiesInDrawOrder[j];
            j = j - 1;
        }
        entitiesInDrawOrder[j + 1] = key;
    }
}

void drawEntities(int layer)
{
    // draw entities into buffer
    for (int i = 0; i < gm.e_len; ++i)
    {
        if (entitiesInDrawOrder[i]->layer == layer)
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

    // Frame index
    uint8_t i = (gm.animation_clock / e->sprite->clock_divider) % e->sprite->length;
    // uint8_t i = gm.animation_clock % e->sprite->length;

    int x = e->x * ENV_UNIT - e->mx;
    int y = e->y * ENV_UNIT - e->my + (ENV_UNIT - e->sprite->frameset[i]->h);

    // TODO: select sprite_frame based on gm.animation_clock
    const uint16_t *sprite_ptr = e->sprite->frameset[i]->addr;

    uint16_t *bufPtr = &buf[y * SCREEN_WIDTH + x];

    // copy sprite into buf.
    for (int row = 0; row < e->sprite->frameset[i]->h; ++row)
    {
        for (int col = 0; col < e->sprite->frameset[i]->w; ++col)
        {
            // Transfer row to buf
            if (*sprite_ptr != COLOR_TRANSPARENT)
                *bufPtr = *sprite_ptr;
            ++bufPtr;
            ++sprite_ptr;
        }
        // Move screen buffer to next row
        bufPtr += SCREEN_WIDTH - e->sprite->frameset[i]->w;
    }
}

/////////////////////////////////////////////////////
// Sprite controls                                ///

bool advance_animation_clock(uint16_t *clock)
{
    static uint32_t last = 0;
    uint32_t now = millis();
    if (now - last >= ANIMATIONSPEED)
    {
        last = now;
        *clock = (*clock + 1);
        return true;
    }
    return false;
}

void updateSpriteTransits()
{
    // Progress all animation cycles with 'step'.
    // must be factor of tile distance
    #define STEP_DISTANCE 4

    for (int i = 0; i < gm.e_len; ++i)
    {
        int8_t x_direction = 0;
        int8_t y_direction = 0;

        Entity *e = &gm.entities[i];
        if (e->mx != 0 || e->my != 0)
        {
            x_direction = (e->mx > 0) - (e->mx < 0); // results in +/- 1 or 0
            y_direction = (e->my > 0) - (e->my < 0); // results in +/- 1 or 0
            // Reduce distance to destination location
            e->mx -= STEP_DISTANCE * x_direction;
            e->my -= STEP_DISTANCE * y_direction;
        }
    }
}



bool spritesInTransit()
{
    for (int i = 0; i < gm.e_len; ++i)
    {
        if (entityInMotion(&gm.entities[i]))
        {
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////

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

    // Wait for movement transistion to complete
    if(spritesInTransit()){
        return false;
    }

    // Test if every target has a crate on the same location.
    for (int i = 0; i < gm.e_len; i++)
    {
        target = &gm.entities[i];
        if (target->type == target_t)
        {
            bool has_crate = false;

            for (int j = 0; j < gm.e_len; j++)
            {
                crate = &gm.entities[j];
                if (
                    crate->type == crate_t ||
                    crate->type == powerconverter_t)
                {
                    if (coLocated(target, crate))
                    {
                        has_crate = true;
                    }
                }
            }
            if (!has_crate)
                return false;
        }
    }
    return true;
}


void screenDrawBuf(uint16_t *buf, int x, int y) { screen.drawCellBuffer(buf, x, y); }
void screenIntro() { screen.drawIntro(); }
void screenSuccess() { screen.drawSuccess(); }
void screenEnvComplete() { screen.drawMapComplete(); }
void screenDemo1() { screen.drawDemo1();}



void testTouchscreen()
{
    // run every 100ms
    static uint16_t last = 0;
    if(millis() - last > 100)
    {
        last = millis();
        boolean istouched = touchscreen.touched();
        if (istouched) {
            TS_Point p = touchscreen.getPoint();
            Serial.print(", x = ");
            Serial.print(p.x);
            Serial.print(", y = ");
            Serial.println(p.y);
        }
    }
}

/////////////////////////////////////////////////////////

void runBehaviours()
{
    for (int i = 0; i < gm.e_len; ++i)
    {
        if (gm.entities[i].behaviour)
        {
            (*gm.entities[i].behaviour)(&gm.entities[i], &gm);
        }
    }
}
