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
int distance;
unsigned long distanceCounter = 0;

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
    unsigned char key = onBoardInfraredSensor.getCode();
    switch (key)
    {
    case BUTTON_A:
        if (mode != BUTTON_A)
        {
            mode = BUTTON_A;
            Serial.println("Mode manuel activé.");
        }
        break;

    case BUTTON_B:
        if (mode != BUTTON_B)
        {
            mode = BUTTON_B;
            Serial.println("Mode autonome activé.");
        }
        break;

    case BUTTON_C:
        if (mode != BUTTON_C)
        {
            mode = BUTTON_C;
            Serial.println("Mode suiveur de ligne activé.");
        }
        break;

    case BUTTON_1:
    {
        int number = map(1, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
    }
    break;

    case BUTTON_2:
    {
        int number = map(2, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_3:
    {
        int number = map(3, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_4:
    {
        int number = map(4, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_5:
    {
        int number = map(5, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_6:
    {
        int number = map(6, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_7:
    {
        int number = map(7, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_8:
    {
        int number = map(8, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    case BUTTON_9:
    {
        int number = map(9, 0, 9, 0, 255);
        if (speed != number)
            speed = number;
        break;
    }

    default:
        break;
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
        int LEDDistance = map(distance, 0, 20, 255, 0);
        if (LEDDistance < 0)
            LEDDistance = 0;
        setLED(0, LEDDistance, 0);

        moveMBot(FORWARD, speed);
        if (distance < 10)
        {
            moveMBot(BACKWARD, speed);
            delay(300);
            moveMBot(RIGHT, speed);
            delay(300);
        }
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

    // Morceau de programme qui lit la distance avec le capteur d'ultrasons toutes les 100ms.
    if (millis() - distanceCounter >= 100)
    {
        distanceCounter = millis();

        distance = onBoardUltrasonicSensor.distanceCm();
    }
}