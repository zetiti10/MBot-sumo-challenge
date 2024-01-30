/**
 * @file main.cpp
 * @author Louis L, Noé M, Lilian A, Paul VB
 * @brief Fichier principal du programme du robot MBot. Cette version spéciale a pour but de ...
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
MeRGBLed onBoardLeftRGBLED(PIN_LEFT_ONBOARD_RGB_LED);
MeRGBLed onBoardRightRGBLED(PIN_RIGHT_ONBOARD_RGB_LED);
MeBuzzer onBoardBuzzer;

// Variables globales.
int counter = 0;
boolean mode = false;

void sendMessage(String message)
{
    delay(10);

    String messageToSend;
    messageToSend = "\033[0;32m";
    messageToSend += message;
    messageToSend += "\033[0m";

    Serial.println(messageToSend);

    delay(10);
}

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    // NE PAS SUPPRIMER LA LIGNE CI-DESSOUS !
    initialization();

    delay(1000);

    /*sendMessage("                 uuuuuuu");
    sendMessage("             uu$$$$$$$$$$$uu");
    sendMessage("          uu$$$$$$$$$$$$$$$$$uu");
    sendMessage("         u$$$$$$$$$$$$$$$$$$$$$u");
    sendMessage("        u$$$$$$$$$$$$$$$$$$$$$$$u");
    sendMessage("       u$$$$$$$$$$$$$$$$$$$$$$$$$u");
    sendMessage("       u$$$$$$$$$$$$$$$$$$$$$$$$$u");
    sendMessage("       u$$$$$$"   "$$$"   "$$$$$$u");
    sendMessage("       u$$$$$$|   |$$$|   |$$$$$$u");
    sendMessage("       |$$$$|      u$u       $$$$|");
    sendMessage("        $$$u       u$u       u$$$");
    sendMessage("        $$$u      u$$$u      u$$$");
    sendMessage("         |$$$$uu$$$   $$$uu$$$$|");
    sendMessage("          |$$$$$$$|   |$$$$$$$|");
    sendMessage("            u$$$$$$$u$$$$$$$u");
    sendMessage("             u$|$|$|$|$|$|$u");
    sendMessage("  uuu        $$u$ $ $ $ $u$$       uuu");
    sendMessage(" u$$$$        $$$$$u$u$u$$$       u$$$$");
    sendMessage("  $$$$$uu      |$$$$$$$$$|     uu$$$$$$");
    sendMessage("u$$$$$$$$$$$uu    |||||    uuuu$$$$$$$$$$");
    sendMessage("$$$$|||$$$$$$$$$$uuu   uu$$$$$$$$$|||$$$|");
    sendMessage(" |||      ||$$$$$$$$$$$uu ||$|||");
    sendMessage("           uuuu ||$$$$$$$$$$uuu");
    sendMessage("  u$$$uuu$$$$$$$$$uu ||$$$$$$$$$$$uuu$$$");
    sendMessage("  $$$$$$$$$$||||           ||$$$$$$$$$$$|");
    sendMessage("   |$$$$$|                      ||$$$$||");
    sendMessage("     $$$|                         $$$$|");

    sendMessage("");*/

    sendMessage("-----------------------------------------");
    sendMessage("|                                       |");
    sendMessage("|              READY TO TROLL           |");
    sendMessage("|                                       |");
    sendMessage("-----------------------------------------");

    for (int i = 0; i < 20000; i += 1)
    {
        onBoardBuzzer.tone(i, 1);
    }
}

void schedule(int frequency)
{
    onBoardBuzzer.tone(frequency, SIREN_SPEED_2);

    counter++;

    if (counter >= LIGHT_SPEED)
    {
        counter = 0;

        if (mode)
        {
            setLeftLED(255, 0, 0);
            setRightLED(0, 0, 255);
            moveMBot(RIGHT, 200);
        }

        else
        {
            setLeftLED(0, 0, 255);
            setRightLED(255, 0, 0);
            moveMBot(LEFT, 200);
        }

        mode = !mode;
    }
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    for (int f = 635; f <= 912; f += SIREN_SPEED_1)
        schedule(f);

    for (int f = 911; f >= 634; f -= SIREN_SPEED_1)
        schedule(f);
}