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
MeRGBLed onBoardLeftRGBLED(PIN_LEFT_ONBOARD_RGB_LED);
MeRGBLed onBoardRightRGBLED(PIN_RIGHT_ONBOARD_RGB_LED);
MeBuzzer onBoardBuzzer;

// Variables globales.
boolean distanceTestMode = false;

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    /*               ZONE À NE PAS MODIFIER               */

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

    /*          FIN DE LA ZONE À NE PAS MODIFIER          */

    // Pour déplacer le robot, utiliser ceci :
    moveMBot(FORWARD, 100);

    // Pour attendre un délai en milliseconde, faire :
    delay(1000);

    // Pour arrêter le robot, mettre la vitesse à 0 :
    moveMBot(FORWARD, 0);

    // Pour faire sonner le buzzer à une fréquence pendant un temps (en ms), faire :
    onBoardBuzzer.tone(900, 100);

    // Pour changer la couleur des DEL RVB, faire :
    setLED(255, 0, 200);

    delay(1000);

    // Pour envoyer un message à l'ordinateur, faire :
    Serial.println("Ça sent la lose chez les autres groupes…");

    Serial.print("…ah non, c'est juste ");
    Serial.println("Paul.");
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    // Test du détecteur de ligne.
    // Un `switch` permet de choisir une option parmis celles disponibles, c'est plus court que des `if … else`. Pour chaque possibilité, un code couleur est associé.
    switch (onBoardLineFinder.readSensors())
    {
    case S1_IN_S2_IN:
        setLED(255, 255, 0);
        break;
    case S1_IN_S2_OUT:
        setLeftLED(255, 255, 0);
        setRightLED(0, 0, 0);
        break;
    case S1_OUT_S2_IN:
        setLeftLED(0, 0, 0);
        setRightLED(255, 255, 0);
        break;
    case S1_OUT_S2_OUT:
        setLED(0, 0, 0);
        break;
    default:
        break;
    }

    if (onBoardInfraredSensor.keyPressed(69))
    {
        for (int i = 0; i < 9000; i++)
        {
            onBoardBuzzer.tone(i, 1);
        }
    }

    if (onBoardInfraredSensor.keyPressed(70))
    {
        for (int i = 9000; i > 0; i--)
        {
            onBoardBuzzer.tone(i, 1);
        }
    }

    // Si le bouton est pressé.
    if (buttonPressed())
    {
        // On attend qu'il soit relaché.
        while (buttonPressed())
            delay(1);

        // Puis on attend un peu par précaution !
        delay(50);

        // Enfin, on active ou on désactive le test du capteur à ultrasons.
        distanceTestMode = !distanceTestMode;
    }

    // Si le mode de test du capteur à ultrasons est activé :
    if (distanceTestMode)
        // On émet un son dont la fréquence dépend de la distance avec l'objet détecté.
        onBoardBuzzer.tone(onBoardUltrasonicSensor.distanceCm(), 100);

    Serial.println(onBoardUltrasonicSensor.distanceCm());
}