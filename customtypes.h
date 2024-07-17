	#include <inttypes.h>
    #include <stdbool.h>

// I had these in engine.h but some issue was causing compile 
// to fail when including engine.h in resources.c!

// Document index of all tiles in any map
// *IMPORTANT* The order of these  match order of tile_ref file (tile_ref_8x8.png)
// cv - convex
// cc - concave
// ol - overlay
typedef enum TileName
{
    void_tn,
    floor_tn,
    stone_tn,
    wall_cv_ne_tn,
    wall_n_tn,
    wall_cv_nw_tn,
    stoneedge_cv_se_tn,
    stoneedge_s_tn,
    stoneedge_sw_tn,
    stoneedge_e_tn,
    stoneedge_w_tn,
    stoneedge_cv_ne_tn,
    stoneedge_n_tn,
    stoneedge_cv_nw_tn,
    stoneedge_cc_wnw_tn,
    stoneedge_cc_nw_tn,
    stoneedge_cc_ne_tn,
    stoneedge_cc_ene_tn,
    stoneedge_cc_wsw_tn,
    stoneedge_cc_sw_tn,
    stoneedge_cc_se_tn,
    stoneedge_cc_ese_tn,
} TileName;

typedef enum TileOverlay
{
    stone_edge_s_to,
    stoneedge_cv_se_to,
    stoneedge_cv_sw_to,
    stoneedge_cc_sw_to,
    stoneedge_cc_se_to,
    null_to,
} TileOverlay;

typedef enum MaterialType
{
    null_t,
    floor_t,
    water_t,
    wall_t,
    goal_t,
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


typedef struct TileSpec
{
    TileName name;
    TileOverlay overlay;
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