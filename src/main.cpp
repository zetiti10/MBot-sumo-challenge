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
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_B))
    {
        mode = BUTTON_B;
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_C))
    {
        mode = BUTTON_C;
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_D))
    {
        mode = BUTTON_D;
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_E))
    {
        mode = BUTTON_E;
    }

    // Exécution du mode en cours.
    if (mode == BUTTON_A)
    {
        Serial.println("mode manuel activé");
        setLED(255, 255, 255);
        if (onBoardInfraredSensor.keyPressed(BUTTON_UP))
        {
            moveMBot(FORWARD, 255);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_DOWN))
        {
            moveMBot(BACKWARD, 255);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_LEFT))
        {
            moveMBot(LEFT, 150);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_RIGHT))
        {
            moveMBot(RIGHT, 150);
        }

        else
        {
            moveMBot(FORWARD, 0);
        }
    }

    else if (mode == BUTTON_B)
    {
        Serial.println("mode détécteur d'obstacles activé");
        setLED(0, 255, 0);
        if (onBoardInfraredSensor.keyPressed(BUTTON_UP))
        {
            moveMBot(FORWARD, 255);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_DOWN))
        {
            moveMBot(BACKWARD, 255);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_LEFT))
        {
            moveMBot(LEFT, 150);
        }

        else if (onBoardInfraredSensor.keyPressed(BUTTON_RIGHT))
        {
            moveMBot(RIGHT, 150);
        }

        else
        {
            moveMBot(FORWARD, 0);
        }
        onBoardBuzzer.tone(onBoardUltrasonicSensor.distanceCm()*1000, 100);
        Serial.println(onBoardUltrasonicSensor.distanceCm());
    }

    else if (mode == BUTTON_C)
    {
        Serial.println("mode suiveur de ligne activé");
    }

    else if (mode == BUTTON_D)
    {
    }

    else if (mode == BUTTON_E)
    {
    }
}