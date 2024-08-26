
#include <cstddef>
#include "interactions.h"

bool atLocation(Entity *entity, int x, int y)
{
    if (entity->x == x && entity->y == y)
    {
        return true;
    }
    return false;
}

bool coLocated(Entity *a, Entity *b)
{
    if (a->x == b->x && a->y == b->y)
    {
        return true;
    }
    return false;
}

struct Entity *entityAtLocation(int x, int y, int layer, GameManager *gm)
{
    for(int i = 0; i < gm->e_len; ++i)
    {
        if(
            gm->entities[i].x == x && 
            gm->entities[i].y == y &&
            gm->entities[i].layer == layer
        )
            return &gm->entities[i];
    }
    return NULL;
}


void moveEntity(Entity *e, int dx, int dy)
{

    // Update entity (x, y) position
    // *** NOTE: entity coordinates updated at immediately: 
    // So subsequent entities moving interact with an up-to-date environment ***
    e->x += dx;
    e->y += dy;
    
    // mx and my are in pixels.
    // they indicate direction and distance sprite has to move
    e->mx += dx * ENV_UNIT;
    e->my += dy * ENV_UNIT;

}