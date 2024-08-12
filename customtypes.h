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
} Entity;

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
    intro,
    inGame,
    success
};


typedef struct EnvSpec
{
    const uint8_t *terrain;
    const Entity *entities;
    const int entity_count;
} EnvSpec;