
#include "Arduino.h"
#include "input.h"


#define DEBOUNCE_THRESHOLD 50





struct BtnHandler {
    int input;
    int current;
    bool processed;
    unsigned long duration;
};

// track button interactions
BtnHandler bh = {.input=-1, .current=-1, .processed=false, .duration=0};

void setupButtonInputs()
{

    pinMode(PIN_MP_PWR_1, OUTPUT);
    pinMode(PIN_MP_PWR_2, OUTPUT);
    pinMode(PIN_MP_PWR_3, OUTPUT);
    digitalWrite(PIN_MP_PWR_1, HIGH);
    digitalWrite(PIN_MP_PWR_2, HIGH);
    digitalWrite(PIN_MP_PWR_3, HIGH);

    pinMode(PIN_MP_N, INPUT_PULLUP);
    pinMode(PIN_MP_E, INPUT_PULLUP);
    pinMode(PIN_MP_S, INPUT_PULLUP);
    pinMode(PIN_MP_W, INPUT_PULLUP);


}

void updateButtonInput(){
    bh.input = -1;
    // Read button set A
    digitalWrite(PIN_MP_PWR_1, LOW);
    if( digitalRead(PIN_MP_N) == LOW){
        bh.input = btn_1_n;
    }
    else if ( digitalRead(PIN_MP_E) == LOW){
        bh.input = btn_1_e;
    }
    else if ( digitalRead(PIN_MP_S) == LOW){
        bh.input = btn_1_s;
    }
    else if ( digitalRead(PIN_MP_W) == LOW){
        bh.input = btn_1_w;
    }
    digitalWrite(PIN_MP_PWR_1, HIGH);

    // Read button set B
    digitalWrite(PIN_MP_PWR_2, LOW);
    if( digitalRead(PIN_MP_N) == LOW){
        bh.input = btn_2_n;
    }
    else if ( digitalRead(PIN_MP_E) == LOW){
        bh.input = btn_2_e;
    }
    else if ( digitalRead(PIN_MP_S) == LOW){
        bh.input = btn_2_s;
    }
    else if ( digitalRead(PIN_MP_W) == LOW){
        bh.input = btn_2_w;
    }
    digitalWrite(PIN_MP_PWR_2, HIGH);

    // Read button set C
    digitalWrite(PIN_MP_PWR_3, LOW);
    if( digitalRead(PIN_MP_N) == LOW){
        bh.input = btn_3_n;
    }
    else if ( digitalRead(PIN_MP_E) == LOW){
        bh.input = btn_3_e;
    }
    else if ( digitalRead(PIN_MP_S) == LOW){
        bh.input = btn_3_s;
    }
    else if ( digitalRead(PIN_MP_W) == LOW){
        bh.input = btn_3_w;
    }
    digitalWrite(PIN_MP_PWR_3, HIGH);

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
        Serial.println(bh.current);
        return bh.current;
    }
}

///////////////////////////////////////////////////////////////
// keypress queue                                           ///

int keypress_queue[KPQMAX];
int size_kpq = 0;
int front_kpq = 0;
int rear_kpq = -1;


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

int kpq_next()
{
    if (size_kpq <= 0)
    {
        return -1;
    }
    return keypress_queue[front_kpq];
}

