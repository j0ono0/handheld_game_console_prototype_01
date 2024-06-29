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


// struct Entity* assetAtLocation(int x, int y, struct Entity* head)
// {
//     struct Entity* current = head;
//     while(current != NULL){
//         if(current->x == x && current->y == y)
//         {
//             return current;
//         }
//         current = current->next;
//     }
//     return NULL;
// }

void moveSprite(int dx, int dy, Entity *entity)
{
    entity->x += dx;
    entity->y += dy;
}

void updateBoxStatus(Entity *box, Entity goals[], int goalsLength)
{
    for(int i = 0; i < goalsLength; i++)
    {
        if(box->x == goals[i].x && box->y == goals[i].y)
        {
            box->color = COLOR_BOX_ACTIVE;
            break;
        }
        else{
            box->color = COLOR_BOX;
        }
    }
}

bool gameSolved(Entity* assetList)
{
    bool solved = true;
    Entity* goal = assetList;
    while(goal != NULL){
        if(goal->type == goal_t){

        }
        goal = goal->next;
    }


    return false;
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
    int count = 0;
    // Store entities at location here
    struct EntityListNode *entitiesHere = NULL;
    struct EntityListNode *current = NULL;

    while(head != NULL){
        if(head->entity->x == x && head->entity->y == y)
        {
            ++count;
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
