#ifndef game_engine_h
#define game_engine_h

#include <inttypes.h>
#include "game_TFT.h"
#include "Arduino.h"
#include "config.h"
#include "customtypes.h"




//////////////////////////////////////////////////////////////////
/// External graphics and environment data in resources.h       //

extern const uint16_t prof_walk_01[];
extern const uint16_t entity_sprites_2[];

extern const uint8_t terrain_tiles_indexed[];
extern const uint16_t terrain_color_table[];


extern const EnvSpec environmentList[];


extern const Sprite sprite_target;
extern const Sprite sprite_crate;
extern const Sprite sprite_crate_active;
extern const Sprite sprite_powerconverter;
extern const Sprite sprite_powerconverter_active;

extern const Sprite sprite_prof_stationary_left;
extern const Sprite sprite_prof_stationary_right;

extern Sprite *prof_walk_east_cycle[];
extern Sprite *prof_walk_west_cycle[];
extern Sprite *prof_walk_north_cycle[];
extern Sprite *prof_walk_south_cycle[];


//////////////////////////////////////////////////////////////////

void setGameMode(GameMode mode);
GameMode gameMode();

int nextEnvironment();
int setEnvironment(int envIndex);

void populateCurrentEntities();
Entity *assignPlayer();

//////////////////////////////////////////////////////////////////
void advanceSpriteAnimations();
void drawAll();
void blitTerrain(uint8_t layer, uint16_t *buf);
uint16_t get_indexed_color(int color_index, int theme_index);
void sortEntityDrawOrder();
void drawEntities(int layer);
void blitEntity(Entity *e, uint16_t *buf);

void moveEntity(Entity *e, int mx, int my);

//////////////////////////////////////////////////////////////////

void updateSprites();
bool spriteInTransit(Entity *e);
bool spritesInTransit();

//////////////////////////////////////////////////////////////////
int tileAtLoc(int x, int y);

bool inbounds(int x, int y);

bool cheatGame(int keypress);
bool gameSolved();

struct Entity *entityAtLocation(int x, int y, int layer);
bool terrainBlocksMovement(int x, int y, int w, int h);

bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);


void screenSetup();
void screenDrawBuf(uint16_t *buf, int x, int y);
void screenIntro();
void screenSuccess();
void screenEnvComplete();

///////////////////////////////////////////////////////////////

void runBehaviours();
bool entity_on_target(Entity *e, GameManager *gm);

#endif