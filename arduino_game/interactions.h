#ifndef game_interactions_h
#define game_interactions_h

//////////////////////////////////////////////////////////////////
//
// functions that are interactions between entities and/or terrain
//
//////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include "customtypes.h"


bool atLocation(Entity *entity, int x, int y);
bool coLocated(Entity *a, Entity *b);
struct Entity *entityAtLocation(int x, int y, int layer, GameManager *gm);
void moveEntity(Entity *e, int dx, int dy);

#endif