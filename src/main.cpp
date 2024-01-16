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

// Autres fichiers du programme.
#include "main.hpp"

// Création des différents composants montés sur le robot.
MeDCMotor leftWheelMotor(9);
MeDCMotor rightWheelMotor(10);
MeRGBLed onBoardRGBLED(7, 2);
MeBuzzer onBoardBuzzer;

/// @brief Démarre un mouvement du robot dans un sens à une vitesse.
/// @param direction Le mouvement (FORWARD = avancer, BACKWARD = reculer, LEFT = tourner à gauche, RIGHT = tourner à droite).
/// @param speed La vitesse, de 0 à 255.
void move(int direction, int speed)
{
    int leftSpeed = 0;
    int rightSpeed = 0;
    
    if (direction == FORWARD)
    {
        leftSpeed = speed;
        rightSpeed = speed;
    }

    else if (direction == BACKWARD)
    {
        leftSpeed = -speed;
        rightSpeed = -speed;
    }

    else if (direction == LEFT)
    {
        leftSpeed = -speed;
        rightSpeed = speed;
    }

    else if (direction == RIGHT)
    {
        leftSpeed = speed;
        rightSpeed = -speed;
    }

    leftWheelMotor.run((9) == M1 ? -(leftSpeed) : (leftSpeed));
    rightWheelMotor.run((10) == M1 ? -(rightSpeed) : (rightSpeed));
}

void setup()
{
    // Pour déplacer le robot, utiliser ceci :
    move(FORWARD, 100);

    // Pour attendre un délai en milliseconde, faire :
    delay(1000);

    // Pour arrêter le robot, mettre la vitesse à 0 :
    move(FORWARD, 0);

    // Pour faire sonner le buzzer à une fréquence pendant un temps, faire :
    onBoardBuzzer.tone(900, 1000);

    // Pour changer la couleur des DEL RVB, faire :
    onBoardRGBLED.setColor(0, 255, 0, 0);
    onBoardRGBLED.setColor(1, 0, 0, 255);
}

void loop()
{
    // On ne fait rien.
    delay(1);
}