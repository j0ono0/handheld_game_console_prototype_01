	#include <inttypes.h>
    #include <stdbool.h>

// I had these in engine.h but some issue was causing compile 
// to fail when including engine.h in resources.c!


// *IMPORTANT* The order of these must match the order of tile_ref file
typedef enum TileRef
{
    // Bases
    missing_tr,
    floor_tr,
    stone_tr,
    water_tr,
    // Features
    stone_front_tr,
    stone_w_tr,
    stone_e_tr,
    stone_nw_tr,
    stone_ne_tr,
    stone_sw_tr,
    stone_se_tr,
    // Compound 
    floor_stone_overhang_tr,
    floor_target_tr,
    water_stone_overhang_tr,
    water_target_tr,

} TileRef;

typedef enum MaterialType
{
    null_t,
    floor_t,
    water_t,
    stone_t,
    stone_front_t,
    stone_w_t,
    stone_e_t,
    stone_ne_t,
    stone_nw_t,
    stone_sw_t,
    stone_se_t,
    stone_overhang_t,
    bench_front_t,
    bench_top_t,
    bench_overhang_t,
    goal_t,
    wall_t,
    plr_t,
    crate_t,
    crate_active_t,
} MaterialType;

typedef struct Entity
{
    int x;
    int y;
    MaterialType type;
} Entity;

typedef enum TileLayer
{
    all_layers,
    base_layer,
    overlay_layer
} TileLayer;

typedef struct TileSpec
{
    TileRef tile;
    MaterialType base;
    MaterialType overlay;
    bool blocks_motion;
} TileSpec;

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
    const int length;
} EnvSpec;