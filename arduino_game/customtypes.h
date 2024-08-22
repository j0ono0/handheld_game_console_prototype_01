	#include <inttypes.h>
    #include <stdbool.h>

// I had these in engine.h but some issue was causing compile 
// to fail when including engine.h in resources.c!

typedef struct Rect
{
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct Dimensions
{
    int w;
    int h;
} Dimensions;

typedef struct Location
{
    int x;
    int y;
} Location;


typedef enum EntityType
{
    plr_t,
    hoodie_t,
    strongman_t,
    officer_t,
    sunlover_t,
    office_chair_t,
    desktop_terminal_t,
    dotpanel_right_t,
    inbuilt_terminal_t,
    desk_clutter_t,
    target_t,
    crate_t,
    crate_active_t,
    powerconverter_t,
    powerconverter_active_t,
    null_t,
} EntityType;

typedef struct SpriteSheet
{
    uint8_t w;
    uint16_t *addr;
} SpriteSheet;

typedef struct Sprite
{
    const uint8_t w;
    const uint8_t h;
    const uint16_t *addr;
} Sprite;


typedef struct Entity
{
    EntityType type;
    int x;
    int y;
    // track if entity renders under (0) or over (1) terrain overlay
    int layer;
    // track movement
    int mx;
    int my;
    void (*behaviour)(struct Entity *e);
    const Sprite *sprite;
} Entity;


//////////////////////////////////////////////////
//
//  'layer' in TileMeta.
//  expected layers currently are:
//  0: base terrain
//  1: entities as 'ground cover'
//  2: entities located on base terrain
//  3: raised terrain (overlaps entities on same location)
//  4: entities located on raised terrain
//
//////////////////////////////////////////////////

typedef struct TileMeta
{
    int layer;
    bool blocks_motion;
} TileMeta;

typedef struct EntitySpecs
{
    EntityType type;
    Dimensions dimensions;
    const uint16_t *sprite_addr;
} EntitySpecs;



enum GameMode
{
    gm_intro,
    gm_inGame,
    gm_success,
    gm_end,
};


typedef struct EnvSpec
{
    const uint8_t *terrain;
    const Entity *entities;
    const uint8_t color_theme;
    const int entity_count;
} EnvSpec;

