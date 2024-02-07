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

// Variables globales
int EtatSuiveur;
int NombreDeLignesFranchies = 0;

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
    while (NombreDeLignesFranchies < 3)
    {
        EtatSuiveur = onBoardLineFinder.readSensors();

        moveMBot(FORWARD, 255);

        if (EtatSuiveur == S1_IN_S2_IN)
        {
            while (EtatSuiveur == S1_IN_S2_IN)
            {
                EtatSuiveur = onBoardLineFinder.readSensors();
            }

            NombreDeLignesFranchies++;
        }
    }

    moveMBot(FORWARD, 0);
}