
#include "Arduino.h"
#include "input.h"


int btnValue;
int flag = 0;

void updateButtonInput(BtnHandler *inputBtn){
    btnValue = 0;
    if( digitalRead(BTN_N) == LOW){
        btnValue = BTN_N;
    }else if ( digitalRead(BTN_W) == LOW){
        btnValue = BTN_W;
    }else if ( digitalRead(BTN_S) == LOW){
        btnValue = BTN_S;
    }else if ( digitalRead(BTN_E) == LOW){
        btnValue = BTN_E;
    }else if ( digitalRead(BTN_SELECT) == LOW){
        btnValue = BTN_SELECT;
    }

    if(millis() - inputBtn->duration < DEBOUNCE_THRESHOLD)
    {
        // waiting for debounce to finish
    }
    else if(inputBtn->current != btnValue)
    {
        // new btn pressed
        inputBtn->current = btnValue;
        inputBtn->processed = false;
        inputBtn->duration = millis();
    }
}

int readUserInput(BtnHandler *inputBtn){

    updateButtonInput(inputBtn);
    
    if (inputBtn->processed == true){
        return 0;
    }
    else{
        inputBtn->processed = true;
        return inputBtn->current;
    }
}
