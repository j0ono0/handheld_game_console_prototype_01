#ifndef game_behaviours_h
#define game_behaviours_h

//////////////////////////////////////////////////////////////////
//
// functions assigned to individual instances of Entities
//
//////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "customtypes.h"
#include "interactions.h"
#include "sprites.h"
#include "input.h"


void do_nothing(Entity *self, GameManager *gm);
void crate_behaviour(Entity *self, GameManager *gm);
void plr_behaviour(Entity *self, GameManager *gm);


#endif