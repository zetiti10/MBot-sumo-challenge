/**
 * @file main.cpp
 * @author Louis L, Noé M, Lilian A, Paul VB
 * @brief Fichier principal du programme du robot MBot.
 * @version 1.0
 * @date 2024-01-16
 */

// Ajout des bibilothèques au programme.
#include <Arduino.h>
#include <MeMCore.h>
#include <SoftwareSerial.h>

// Autres fichiers du programme.
#include "main.hpp"
#include "pinDefinitions.hpp"
#include "functions.hpp"

// Création des différents capteurs montés sur le robot.
MeLineFollower onBoardLineFinder(PORT_1);
MeUltrasonicSensor onBoardUltrasonicSensor(PIN_ONBOARD_ULTRASONIC_SENSOR);
MeIR onBoardInfraredSensor;

// Création des différents actionneurs montés sur le robot.
MeDCMotor leftWheelMotor(PIN_LEFT_WHEEL_MOTOR);
MeDCMotor rightWheelMotor(PIN_RIGHT_WHEEL_MOTOR);
MeRGBLed onBoardRGBLED(PIN_RIGHT_ONBOARD_RGB_LED, PIN_LEFT_ONBOARD_RGB_LED);
MeBuzzer onBoardBuzzer;

// Variables globales.
int counter = 0;

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    // NE PAS SUPPRIMER LA LIGNE CI-DESSOUS !
    initialization();

    // Programme exécuté une fois au démarrage du robot.
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    // Programme exécuté en boucle.
    if (onBoardInfraredSensor.keyPressed(BUTTON_UP))
    {
        moveMBot(FORWARD, 255);
        setLED(255, 255, 255);
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_DOWN))
    {
        moveMBot(BACKWARD, 255);
        setLED(255, 0, 0);
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_LEFT))
    {
        moveMBot(LEFT, 150);
        setLED(0, 255, 0);
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_RIGHT))
    {
        moveMBot(RIGHT, 150);
        setLED(0, 0, 255);
    }

    else
    {
        moveMBot(FORWARD, 0);
        setLED(0, 0, 0);
    }
}