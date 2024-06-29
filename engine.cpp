#include "engine.h"


bool inbounds(int x, int y)
{
    if(x < 0 || y < 0 || x > GRID_WIDTH || y > GRID_HEIGHT)
    {
        Serial.print(x);
        return false;
    }
    return true;
}


void moveSprite(int dx, int dy, Entity *entity)
{
    entity->x += dx;
    entity->y += dy;
}

void updateCrate(struct EntityListNode *head, Entity *crate)
{
    crate->type = crate_t;
    while(head != NULL){
        if(head->entity->x == crate->x && head->entity->y == crate->y && head->entity->type == goal_t )
        {
            crate->type = crate_active_t;
        }
        head = head->next;
    }
}

bool gameSolved(struct EntityListNode *head)
{
    // Test if every goal has a crate on the same location.
    bool solved = true;
    struct EntityListNode* current = head;
    while(current != NULL)
    {
        if(current->entity->type == goal_t){
            solved = false;
            struct EntityListNode* test = head;
            while(test != NULL)
            {
                if((test->entity->type == crate_active_t || test->entity->type == crate_t) && test->entity->x == current->entity->x && test->entity->y == current->entity->y)
                {
                    solved = true;
                    break;
                }
                test = test->next;
            }
        }
        if(solved == false){
            break;
        }
        current = current->next;
    }
    return solved;
}

struct Entity* appendAsset(struct Entity** head, int  col, int row,  enum EntityType type, uint16_t color)
{
    struct Entity* newNode = (struct Entity*)malloc(sizeof(struct Entity));
    newNode->x = col;
    newNode->y = row;
    newNode->type = type;
    newNode->color = color;
    newNode->next = NULL;

    if(*head == NULL)
    {
        *head = newNode;
        return newNode;
    }
    struct Entity* current = *head;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return newNode;
}


struct Entity *createAsset(struct EntityListNode **head, enum EntityType type, int x, int y, uint16_t color)
{
    // // Create entity
    struct Entity *e = (struct Entity *) malloc(sizeof(struct Entity));
    e->type = type;
    e->x = x;
    e->y = y;
    e->color = color;
    // Create linked-list node
    struct EntityListNode *node = (struct EntityListNode *) malloc(sizeof(struct EntityListNode));
    node->entity = e;
    node->next = NULL;

    // Add node into provided linked-list (head)
    // List is empty
    if(*head == NULL){
        *head = node;
        return e;
    }
    // find last list item
    struct EntityListNode *temp = *head;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = node;
    return e;
}

struct Entity* entityAtLocation(struct EntityListNode** head, int x, int y)
{
    struct EntityListNode* temp = *head;

    while(temp != NULL){
        if(temp->entity->x == x && temp->entity->y == y)
        {
            return temp->entity;
        }
        temp = temp->next;
    }
    return NULL;
}

struct EntityListNode* entitiesAtLocation(struct EntityListNode* head, int x, int y)
{
    // Store entities at location here
    struct EntityListNode *entitiesHere = NULL;
    struct EntityListNode *current = NULL;

    while(head != NULL){
        if(head->entity->x == x && head->entity->y == y)
        {
            // make a new entity list entry
            if (entitiesHere == NULL){
                entitiesHere = (struct EntityListNode *) malloc(sizeof(struct EntityListNode));
                entitiesHere->entity = head->entity;
                entitiesHere->next = NULL;
                current = entitiesHere;
            }else{
                struct EntityListNode *node = (struct EntityListNode *) malloc(sizeof(struct EntityListNode));
                node->entity = head->entity;
                current->next = node;
                current = current->next;
            }
        }
        head = head->next;
    }
    return entitiesHere;
}

bool entityBlocksMovement(struct EntityListNode* head, int x, int y)
{
    while(head != NULL){
        if(head->entity->x == x && head->entity->y == y && head->entity->type == wall_t){
            return true;
        }
        head = head->next;
    }
    return false;
}

struct Entity* crateAtLocation(struct EntityListNode* head, int x, int y)
{
    while(head != NULL){
        if( head->entity->x == x && head->entity->y == y && (head->entity->type == crate_t || head->entity->type == crate_active_t))
        {
            return head->entity;
        }
        head = head->next;
    }
    return NULL;
}

void deleteAssets(struct EntityListNode* assets)
{
    struct EntityListNode *current = NULL;
    while(assets != NULL)
    {
        current = assets;
        assets = assets->next;
        free(current);
    }
    Serial.println("finished removing assets");
}