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





struct Entity* assetAtLocation(int x, int y, struct Entity* head)
{
    struct Entity* current = head;
    while(current != NULL){
        if(current->x == x && current->y == y)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

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


void prependEntity(struct Entity** head, struct Entity* entity)
{
    /* Add entry to *start* of linked list */
    struct EntityListNode* new_node;
    new_node = (EntityListNode *) malloc(sizeof(EntityListNode));
    new_node->entity = entity;
    new_node->next = head*;
    head* = new_node;
}

void appendEntity(struct Entity** head, struct Entity* entity)
{
    /* Add entry to *start* of linked list */
    struct EntityListNode* new_node;
    new_node = (EntityListNode *) malloc(sizeof(EntityListNode));
    new_node->entity = entity;

    struct EntityListNode *t = *head;
    while(t->next != NULL)
    {
        t = t->next;
    }
    t->next = new_node;
    new_node->next = NULL;
}