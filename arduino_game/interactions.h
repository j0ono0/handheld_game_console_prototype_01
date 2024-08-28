#ifndef game_interactions_h
#define game_interactions_h

//////////////////////////////////////////////////////////////////
//
// functions that are interactions between entities and/or terrain
//
//////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include "customtypes.h"

extern const TileMeta tile_meta[];

bool inbounds(int x, int y);
int tileAtLoc(int x, int y, GameManager *gm);
bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);
bool terrainBlocksMovement(int x, int y, int w, int h, GameManager *gm);
struct Entity *entityAtLocation(int x, int y, int layer, GameManager *gm);
void moveEntity(Entity *e, int dx, int dy);
bool entity_on_target(Entity *e, GameManager *gm);
bool entityInMotion(Entity *e);

#endif