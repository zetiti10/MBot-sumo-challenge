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

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    // NE PAS SUPPRIMER LA LIGNE CI-DESSOUS !
    initialization();

    // Programme exécuté une fois au démarrage du robot.
    while (!buttonPressed())
        delay(1);
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{

    // On attend qu'il soit relaché.

    // Puis on attend un peu par précaution !
    delay(50);

    if (onBoardInfraredSensor.keyPressed(BUTTON_A))
    {
        setLeftLED(255, 0, 0);
        Serial.println("Bouton A cliqué");
    }

    else if (onBoardInfraredSensor.keyPressed(BUTTON_B))
    {
        setRightLED(10, 150, 0);
        Serial.println("Bouton B cliqué");
    }

    else 
        setLED(0, 0, 0);
}