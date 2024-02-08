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
int speed = 155;
unsigned long mainScheduler = 0;

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    // NE PAS SUPPRIMER LA LIGNE CI-DESSOUS !
    initialization();

    // Programme exécuté une fois au démarrage du robot.
    delay(1000);
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    // Programme exécuté en boucle.
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

    // Une fois toutes les 100ms, on vérifie les capteurs.
    if (millis() - mainScheduler >= 100)
    {
        mainScheduler = millis();

        // Gagné.
        if (onBoardUltrasonicSensor.distanceCm() < 15)
        {
            moveMBot(FORWARD, 0);

            setLED(0, 0, 255);
            onBoardBuzzer.tone(1000, 100);

            // Communication avec le concurrent.
            for (int i = 0; i < 10; i++)
                onBoardInfraredSensor.sendString("stop");

            while (!buttonPressed())
            {
                setLED(0, 0, 255);
                onBoardBuzzer.tone(1000, 100);
                setLED(255, 255, 255);
                onBoardBuzzer.tone(1500, 100);
                setLED(255, 0, 0);
                onBoardBuzzer.tone(200, 100);
            }
        }

        // Détection des autres messages.
        if (onBoardInfraredSensor.getString() != "")
        {
            moveMBot(FORWARD, 0);

            while (!buttonPressed())
            {
                setLED(255, 0, 0);
                delay(500);
            }
        }
    }
}