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


// Document index of all tiles in any map
// *IMPORTANT* The order of these  match order of tile_ref file (tile_ref_8x8.png)
// cv - convex
// cc - concave
// ol - overlay
typedef enum TileName
{
    void_t,
    floor_tn,
    wall_tn,
    wallcnr_cc_nw_tn,
    wallcnr_cc_ne_tn,
    wall_n_tn,
    wall_w_tn,
    wall_e_tn,
    wallcnr_cc_sw_tn,
    wallcnr_cc_ssw_tn,
    wall_s_tn,
    wallcnr_cc_se,
    wallcnr_cv_se,
    wallcnr_cv_sw,
    wallcnr_cv_ne_tn,
    wallcnr_cv_nw_tn,
    shadow_n_tn,
    shadow_w_tn,
    shadow_cc_nw_tn,
    shadow_cv_nw_tn,
    shadow_cv_ne_tn,
    shadow_cv_sw_tn,
} TileName;

typedef enum TileOverlay
{
    wall_s_to,
    wall_cv_se_to,
    wall_cv_sw_to,
    null_to,
} TileOverlay;

typedef enum EntityType
{
    plr_t,
    crate_t,
    crate_active_t,
    target_t,
    null_t,
} EntityType;

typedef struct Entity
{
    EntityType type;
    int x;
    int y;
    // track movement
    int mx;
    int my;
    void (*behaviour)(struct Entity *e);
} Entity;

typedef struct TileSpec
{
    TileName name;
    TileOverlay overlay;
    bool blocks_motion;
} TileSpec;

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