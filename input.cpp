
#include "Arduino.h"
#include "input.h"

// track button interactions
BtnHandler bh = {.input=0, .current=0, .processed=false, .duration=0};

void setupButtonInputs()
{
    pinMode(BTN_N, INPUT_PULLUP);
    pinMode(BTN_E, INPUT_PULLUP);
    pinMode(BTN_S, INPUT_PULLUP);
    pinMode(BTN_W, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
}

void updateButtonInput(){
    bh.input = 0;
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
        return 0;
    }
    else{
        bh.processed = true;
        return bh.current;
    }
}
