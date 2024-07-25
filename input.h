#ifndef game_inputButtons_h
#define game_inputButtons_h


#define BTN_N 5
#define BTN_W 4
#define BTN_S 3
#define BTN_E 6
#define BTN_SELECT 7

#define DEBOUNCE_THRESHOLD 50

struct BtnHandler {
    int input;
    int current;
    bool processed;
    unsigned long duration;
};

void setupButtonInputs();
void updateButtonInput();
int readUserInput();

// queue for user input (keypresses)
#define KPQMAX 5
int keypress_queue_length();
void enqueue_kpq(int keyvalue);
int dequeue_kpq();
int queue_length();

#endif
