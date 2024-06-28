#ifndef game_engine_h
#define game_engine_h

#include <cstdint>
#include "game_TFT.h"

    
    struct Loc {
        int x;
        int y;
    };

    enum EntityType {
        plr_t,
        floor_t,
        wall_t,
        crate_t,
        crate_active_t,
        goal_t
    };

    enum GameMode {
        intro,
        inGame,
        success
    };


    struct Entity {
        int x;
        int y;
        uint16_t color;
        enum EntityType type;
        struct Entity* next;
    };


    bool inbounds(int x, int y);
    Entity *entityAtLocation(int x, int y, struct Entity array[], int len);
    void moveSprite(int dx, int dy, Entity *entity);
    void updateBoxStatus(Entity *box, Entity goals[], int goalsLength);
    bool gameSolved(Entity boxes[], int boxCount, Entity goals[], int goalCount);
    struct Entity* appendAsset(struct Entity** head, int  col, int row, EntityType, uint16_t color);
#endif