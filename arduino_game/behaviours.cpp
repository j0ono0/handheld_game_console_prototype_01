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
    if(
        self->sprite == &sprite_crate_1 || 
        self->sprite == &sprite_crate_2 ||
        self->sprite == &sprite_crate_3 ||
        self->sprite == &sprite_crate_4 ||
        self->sprite == &sprite_crate_5 ||
        self->sprite == &sprite_crate_6 
    )
    {
        return;
    }
    if(entity_on_target(self, gm) && self->mx == 0 && self->my == 0)
    {
        self->sprite = &sprite_crate_active;
        return;
    }
    self->sprite = &sprite_crate;
}

void powerconverter_behaviour(Entity *self, GameManager *gm)
{
    if(entity_on_target(self, gm))
    {
        self->sprite = &sprite_powerconverter_active;
        return;
    }
    self->sprite = &sprite_powerconverter;
}

void plr_behaviour(Entity *self, GameManager *gm)
{
    int next_input = dequeue_kpq();
    int dx = 0;
    int dy = 0;
    // if input aborted revert to prev_sprite
    const Sprite *prev_sprite = self->sprite;

    switch (next_input)
    {
    case btn_a_n:
        dy = -1;
        self->sprite = &sprite_prof_walk_north;
        break;
    case btn_a_s:
        dy = 1;
        self->sprite = &sprite_prof_walk_south;
        break;
    case btn_a_w:
        dx = -1;
        self->sprite = &sprite_prof_walk_west;
        break;
    case btn_a_e:
        dx = 1;
        self->sprite = &sprite_prof_walk_east;
        break;
    default:
        // Walking transition has completed and no user input.
        if(self->sprite == &sprite_prof_walk_north)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        else if(self->sprite == &sprite_prof_walk_east)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        else if(self->sprite == &sprite_prof_walk_south)
        {
            self->sprite = &sprite_prof_stationary_east;
        }
        else if(self->sprite == &sprite_prof_walk_west)
        {
            self->sprite = &sprite_prof_stationary_west;
        }
        return;
    }

    int nextX = self->x + dx;
    int nextY = self->y + dy;

    if (!inbounds(nextX, nextY))
    {
        self->sprite = prev_sprite;
        Serial.println("Out of bounds!");
        return;
    }
    else if (terrainBlocksMovement(nextX, nextY, 2, 2, gm))
    {
        self->sprite = prev_sprite;
        Serial.println("There is no way through here.");
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
}
