#ifndef FUNCTIONS_DEFINITIONS
#define FUNCTIONS_DEFINITIONS

#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

void moveMBot(int direction, int speed);
void setLeftLED(int r, int g, int b);
void setRightLED(int r, int g, int b);
void setLED(int r, int g, int b);
boolean buttonPressed();

#endif