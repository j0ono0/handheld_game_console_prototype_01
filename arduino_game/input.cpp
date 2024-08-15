
#include "Arduino.h"
#include "input.h"

// track button interactions
BtnHandler bh = {.input=-1, .current=-1, .processed=false, .duration=0};

void setupButtonInputs()
{
    pinMode(BTN_N, INPUT_PULLUP);
    pinMode(BTN_E, INPUT_PULLUP);
    pinMode(BTN_S, INPUT_PULLUP);
    pinMode(BTN_W, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
}

void updateButtonInput(){
    bh.input = -1;
    if( digitalRead(BTN_N) == LOW){
        bh.input = BTN_N;
    }else if ( digitalRead(BTN_W) == LOW){
        bh.input = BTN_W;
    }else if ( digitalRead(BTN_S) == LOW){
        bh.input = BTN_S;
    }else if ( digitalRead(BTN_E) == LOW){
        bh.input = BTN_E;
    }else if ( digitalRead(BTN_SELECT) == LOW){
        bh.input = BTN_SELECT;
    }

    if(millis() - bh.duration < DEBOUNCE_THRESHOLD)
    {
        // waiting for debounce to finish
    }
    else if(bh.current != bh.input)
    {
        // new btn pressed
        bh.current = bh.input;
        bh.processed = false;
        bh.duration = millis();
    }
}

int readUserInput(){

    updateButtonInput();
    
    if (bh.processed == true){
        return -1;
    }
    else{
        bh.processed = true;
        return bh.current;
    }
}

///////////////////////////////////////////////////////////////
// keypress queue                                           ///

int keypress_queue[KPQMAX];
int size_kpq = 0;
int front_kpq = 0;
int rear_kpq = -1;

int kpq_length()
{
    return rear_kpq - front_kpq;
}

void enqueue_kpq(int keyvalue)
{
    if (keyvalue < 0)
    {
        // valid values are equal or above 0 
        return;
    }
    if(size_kpq == KPQMAX)
    {
       Serial.println("Keypress queue full!");
    }
    else
    {
        rear_kpq = (rear_kpq + 1) % KPQMAX; // neat way to make circluar!
        keypress_queue[rear_kpq] = keyvalue;
        size_kpq++;
    }
} 
 
int dequeue_kpq()
{
    if(size_kpq <= 0)
    {
        return -1;
    }
    int value = keypress_queue[front_kpq];
    front_kpq = (front_kpq + 1) % KPQMAX;
    size_kpq--;
    return value;
}

int queue_length()
{
    return size_kpq;
}