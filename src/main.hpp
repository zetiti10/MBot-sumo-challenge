#ifndef MAIN_DEFINITIONS
#define MAIN_DEFINITIONS

#include <MeMCore.h>
#include <SoftwareSerial.h>

#define BUTTON_A 69
#define BUTTON_B 70
#define BUTTON_C 71
#define BUTTON_D 68
#define BUTTON_E 67
#define BUTTON_F 13
#define BUTTON_UP 64
#define BUTTON_DOWN 25
#define BUTTON_LEFT 7
#define BUTTON_RIGHT 9
#define BUTTON_0 22
#define BUTTON_1 12
#define BUTTON_2 14
#define BUTTON_3 94
#define BUTTON_4 8
#define BUTTON_5 28
#define BUTTON_6 98
#define BUTTON_7 66
#define BUTTON_8 82
#define BUTTON_9 74
#define BUTTON_SETTINGS 21

// Configuration du programme.
#define SIREN_SPEED_1 3
#define SIREN_SPEED_2 1
#define LIGHT_SPEED 10

// Création des différents capteurs montés sur le robot.
extern MeLineFollower onBoardLineFinder;
extern MeUltrasonicSensor onBoardUltrasonicSensor;
extern MeIR onBoardInfraredSensor;

// Création des différents actionneurs montés sur le robot.
extern MeDCMotor leftWheelMotor;
extern MeDCMotor rightWheelMotor;
extern MeRGBLed onBoardLeftRGBLED;
extern MeRGBLed onBoardRightRGBLED;
extern MeBuzzer onBoardBuzzer;

#endif