#ifndef game_inputButtons_h
#define game_inputButtons_h


#define BTN_N 5
#define BTN_W 4
#define BTN_S 3
#define BTN_E 6
#define BTN_SELECT 7

#define DEBOUNCE_THRESHOLD 50

struct BtnHandler {
    int current;
    bool processed;
    unsigned long duration;
};

void updateButtonInput(BtnHandler *inputBtn);
int readUserInput(BtnHandler *inputBtn);
#endif
