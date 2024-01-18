#ifndef MAIN_DEFINITIONS
#define MAIN_DEFINITIONS

#include <MeMCore.h>
#include <SoftwareSerial.h>

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