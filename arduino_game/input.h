#ifndef game_inputButtons_h
#define game_inputButtons_h

#include "config.h"
#include "customtypes.h"


void setupButtonInputs();
void updateButtonInput();
// void readBtnSet(int pin);
int readUserInput();


// queue for user input (keypresses)
#define KPQMAX 5
void enqueue_kpq(int keyvalue);
int dequeue_kpq();
int kpq_next();

#endif
