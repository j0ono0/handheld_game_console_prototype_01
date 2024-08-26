#include "behaviours.h"


void do_nothing(Entity *self, GameManager *gm){}


void crate_behaviour(Entity *self, GameManager *gm)
{
    //Serial.println("crate behaving");
    self->sprite = &sprite_crate;
    for(int i = 0; i < gm->e_len; ++i)
    {
        if(gm->entities[i].type == target_t && coLocated(self, &gm->entities[i])){
            self->sprite = &sprite_crate_active;
            break;
        }
    }
}

void plr_behaviour(Entity *self, GameManager *gm)
{
    int next_input = dequeue_kpq();
    int dx = 0;
    int dy = 0;

    switch (next_input)
    {
    case BTN_N:
        dy = -1;
        break;
    case BTN_S:
        dy = 1;
        break;
    case BTN_W:
        dx = -1;
        break;
    case BTN_E:
        dx = 1;
        break;
    default:
     // No valid user input.
        return;
    }

    int nextX = self->x + dx;
    int nextY = self->y + dy;

    // if (!inbounds(nextX, nextY))
    // {
    //     Serial.println("Out of bounds!");
    //     return;
    // }
    // else if (terrainBlocksMovement(nextX, nextY, 2, 2))
    // {
    //     Serial.println("There is no way through here.");
    //     return;
    // }

    // Entity *target_entity = entityAtLocation(nextX, nextY, self->layer);
    // if(target_entity && target_entity->layer == self->layer)
    // {
    //     // Test if tile after is free
    //     nextX += dx;
    //     nextY += dy;

    //     if(terrainBlocksMovement(nextX, nextY, 2, 2))
    //     {
    //         return;
    //     }

    //     Entity *entity_at_next_loc = entityAtLocation(nextX, nextY, target_entity->layer);

    //     if(entity_at_next_loc && entity_at_next_loc->layer == target_entity->layer)
    //     {
    //         Serial.println("entity blocks way.");
    //         // target entity cannot move, so plr cannot move
    //         return; 
    //     }
    //     moveEntity(target_entity, dx, dy);
    // }

    moveEntity(self, dx, dy);


}
