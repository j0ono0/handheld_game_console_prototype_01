#include "behaviours.h"


void updateMovementTransition(Entity *e, uint8_t step_distance)
{
    // TODO: If STEP_DISTANCE disallows sprite to end on zero, sprite will oscillate???
    if (e->mx == 0 || e->my == 0)
    {
        return;
    }

    int8_t x_direction = (e->mx > 0) - (e->mx < 0); // results in +/- 1 or 0
    int8_t y_direction = (e->my > 0) - (e->my < 0); // results in +/- 1 or 0
    // Reduce distance to destination location
    e->mx -= step_distance * x_direction;
    e->my -= step_distance * y_direction;
}


void do_nothing(Entity *self, GameManager *gm) {}

void crate_behaviour(Entity *self, GameManager *gm)
{
    Serial.println("crate behaving");
    self->sprite = &sprite_crate;
    for (int i = 0; i < gm->e_len; ++i)
    {
        if (gm->entities[i].type == target_t && coLocated(self, &gm->entities[i]))
        {
            self->sprite = &sprite_crate_active;
            break;
        }
    }
}

void powerconverter_behaviour(Entity *self, GameManager *gm)
{
    // self->sprite = &sprite_powerconverter;
    // for (int i = 0; i < gm->e_len; ++i)
    // {
    //     if (gm->entities[i].type == target_t && coLocated(self, &gm->entities[i]))
    //     {
    //         self->sprite = &sprite_powerconverter_active;
    //         break;
    //     }
    // }
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

    if (!inbounds(nextX, nextY))
    {
        Serial.println("Out of bounds!");
        ///////////////////////////////////////
        //
        // TODO: much repeated status selection - make more DRY!!!
        //
        ///////////////////////////////////////
        if(dx < 0)
        {
            self->sprite = &sprite_prof_stationary_west;
        }
        else if(dx > 0)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        else if(dy < 0)
        {
            self->sprite = &sprite_prof_stationary_west;
        }
        else if(dy > 0)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        return;
    }
    else if (terrainBlocksMovement(nextX, nextY, 2, 2, gm))
    {
        Serial.println("There is no way through here.");
        if(dx < 0)
        {
            self->sprite = &sprite_prof_stationary_west;
        }
        else if(dx > 0)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        else if(dy < 0)
        {
            self->sprite = &sprite_prof_stationary_west;
        }
        else if(dy > 0)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        return;
    }

    Entity *target_entity = entityAtLocation(nextX, nextY, self->layer, gm);
    if (target_entity && target_entity->layer == self->layer)
    {
        // Test if tile after is free
        nextX += dx;
        nextY += dy;

        if (terrainBlocksMovement(nextX, nextY, 2, 2, gm))
        {
            return;
        }

        Entity *entity_at_next_loc = entityAtLocation(nextX, nextY, target_entity->layer, gm);

        if (entity_at_next_loc && entity_at_next_loc->layer == target_entity->layer)
        {
            Serial.println("entity blocks way.");
            // target entity cannot move, so plr cannot move
            return;
        }
        moveEntity(target_entity, dx, dy);
    }

    moveEntity(self, dx, dy);

    // Set status: used for sprite selection
    if(dx < 0)
    {
        self->sprite = &sprite_prof_walk_west;
    }
    else if(dx > 0)
    {
        self->sprite = &sprite_prof_walk_east;
    }
    else if(dy < 0)
    {
        self->sprite = &sprite_prof_walk_north;
    }
    else if(dy > 0)
    {
        self->sprite = &sprite_prof_walk_south;
    }
}
