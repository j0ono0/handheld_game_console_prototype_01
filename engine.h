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

    // Linked lists
    struct EntityListNode {
        struct Entity* entity;
        struct EntityListNode* next;
    };


    bool inbounds(int x, int y);
    void moveSprite(int dx, int dy, Entity *entity);
    void updateCrate(struct EntityListNode *head, Entity *crate);
    bool gameSolved(struct EntityListNode *head);
    struct Entity* appendAsset(struct Entity** head, int  col, int row, EntityType, uint16_t color);

    struct Entity *createAsset(struct EntityListNode **head, enum EntityType type, int x, int y, uint16_t color);
    struct Entity* entityAtLocation(struct EntityListNode* head, int x, int y);
    struct EntityListNode* entitiesAtLocation(struct EntityListNode* head, int x, int y);
    bool entityBlocksMovement(struct EntityListNode* head, int x, int y);
    struct Entity* crateAtLocation(struct EntityListNode* head, int x, int y);
    void deleteAssets(struct EntityListNode* assets);
#endif