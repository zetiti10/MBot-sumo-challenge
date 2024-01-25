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
int mode = BUTTON_A;
int speed = 100;

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

    // Sélection du mode.
    if (onBoardInfraredSensor.keyPressed(BUTTON_A))
    {
        mode = BUTTON_A;
        Serial.println("Mode manuel activé.");
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_B))
    {
        mode = BUTTON_B;
        Serial.println("Mode détécteur d'obstacles activé.");
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_C))
    {
        mode = BUTTON_C;
        Serial.println("Mode suiveur de ligne activé.");
    }

    if (onBoardInfraredSensor.keyPressed(BUTTON_D))
    {
        speed = 100;
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_E))
    {
        speed = 255;
    }

    // Exécution du mode en cours.
    if (mode == BUTTON_A)
    {
        setLED(255, 255, 255);
        if (onBoardInfraredSensor.keyPressed(BUTTON_UP))
        {
            moveMBot(FORWARD, speed);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_DOWN))
        {
            moveMBot(BACKWARD, speed);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_LEFT))
        {
            moveMBot(LEFT, speed);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_RIGHT))
        {
            moveMBot(RIGHT, speed);
        }

        else
        {
            moveMBot(FORWARD, 0);
        }
    }

    else if (mode == BUTTON_B)
    {
        int distance = map(onBoardUltrasonicSensor.distanceCm(), 0, 50, 255, 0);
        if(distance < 0) distance = 0;
        setLED(0, distance, 0);
        delay(100);
    }

    else if (mode == BUTTON_C)
    {
        switch (onBoardLineFinder.readSensors())
        {
        case S1_IN_S2_IN:
            setLED(255, 255, 0);
            moveMBot(FORWARD, speed);
            break;
        case S1_IN_S2_OUT:
            setLeftLED(255, 255, 0);
            setRightLED(0, 0, 0);
            moveMBot(LEFT, speed);
            break;
        case S1_OUT_S2_IN:
            setLeftLED(0, 0, 0);
            setRightLED(255, 255, 0);
            moveMBot(RIGHT, speed);
            break;
        case S1_OUT_S2_OUT:
            setLED(0, 0, 0);
            moveMBot(BACKWARD, speed);
            break;
        default:
            break;
        }
    }
}