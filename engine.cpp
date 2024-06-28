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

Entity *entityAtLocation(int x, int y, struct Entity array[2], int len)
{
    for(int i = 0; i < len; i++)
    {
        if(array[i].x == x && array[i].y == y){
            return &array[i];
        }
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
            box->color = COLOR_BOX_TARGET;
            break;
        }
        else{
            box->color = COLOR_BOX;
        }
    }
}

bool gameSolved(Entity boxes[], int boxCount, Entity goals[], int goalCount)
{
    int count = 0;
    // Check if all goals are covered by boxes
    for(int b = 0; b < boxCount; b++){
        for(int g = 0; g < goalCount; g++){
            if(boxes[b].x == goals[g].x && boxes[b].y == goals[g].y)
            {
                count++;
                break;
            }
        }
    }
    if (count == goalCount)
    {
        return true;
    }
    return false;
}

struct Entity* appendAsset(struct Entity** head, int  col, int row,  enum EntityType, uint16_t color)
{
    struct Entity* newNode = (struct Entity*)malloc(sizeof(struct Entity));
    newNode->x = col;
    newNode->y = row;
    newNode->color = color;
    newNode->next = NULL;

    if(*head == NULL)
    {
        *head = newNode;
        return *head;
    }
    struct Entity* current = *head;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return current;
}