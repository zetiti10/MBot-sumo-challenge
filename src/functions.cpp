/**
 * @file functions.cpp
 * @author Louis L, Noé M, Lilian A, Paul VB
 * @brief Fichier contenant les fonctions permettant d'utiliser les différents composants du robot.
 * @version 1.0
 * @date 2024-01-16
 */

// Ajout des bibilothèques au programme.
#include <Arduino.h>
#include <MeMCore.h>
#include <SoftwareSerial.h>

// Autres fichiers du programme.
#include "functions.hpp"
#include "pinDefinitions.hpp"
#include "main.hpp"

void initialization()
{
    // Démarrage de la communication avec l'ordinateur.
    Serial.begin(115200);
    Serial.println("Connexion établie !");
    Serial.println("Début de l'initialisation des composants du MBot...");

    // Définition des broches des capteurs.
    pinMode(PIN_ONBOARD_BUTTON, INPUT);

    Serial.println("Broche des capteurs initialisées.");

    // Lancement du capteur infrarouge (pour détecter les appuis de la télécommande).
    onBoardInfraredSensor.begin();

    Serial.println("Capteur infrarouges initialisé.");

    Serial.println("Initialisation terminée avec succès !");
}

/// @brief Modifie l'état des moteurs des roues du MBot. Pour arrêter le robot, mettre la vitesse à 0.
/// @param direction Le déplacement à effectuer (FORWARD = avancer, BACKWARD = reculer, LEFT = tourner à gauche, RIGHT = tourner à droite).
/// @param speed La vitesse, de -255 à 255.
void moveMBot(int direction, int speed)
{
    if (speed < -255)
        speed = -255;
    if (speed > 255)
        speed = 255;

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

    leftWheelMotor.run(-(leftSpeed));
    rightWheelMotor.run(rightSpeed);
}

/// @brief Définit la couleur de la DEL RVB gauche.
/// @param r La luminosité du rouge (de 0 à 255).
/// @param g La luminosité du vert (de 0 à 255).
/// @param b La luminosité du bleu (de 0 à 255).
void setLeftLED(int r, int g, int b)
{
    onBoardRGBLED.setColor(2, r, g, b);
    onBoardRGBLED.show();
}

/// @brief Définit la couleur de la DEL RVB droite.
/// @param r La luminosité du rouge (de 0 à 255).
/// @param g La luminosité du vert (de 0 à 255).
/// @param b La luminosité du bleu (de 0 à 255).
void setRightLED(int r, int g, int b)
{
    onBoardRGBLED.setColor(1, r, g, b);
    onBoardRGBLED.show();
}

/// @brief Définit la couleur des deux DEL RVB.
/// @param r La luminosité du rouge (de 0 à 255).
/// @param g La luminosité du vert (de 0 à 255).
/// @param b La luminosité du bleu (de 0 à 255).
void setLED(int r, int g, int b)
{
    setLeftLED(r, g, b);
    setRightLED(r, g, b);
}

/// @brief Fonction permettant de savoir si le bouton du MBot est pressé.
/// @return Renvoie `true` si le bouton est pressé.
boolean buttonPressed()
{
    if (analogRead(PIN_ONBOARD_BUTTON) < 10) // A vérifier !
        return true;

    else
        return false;
}