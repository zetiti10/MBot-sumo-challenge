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
#include "pinDefinitions.hpp"
#include "functions.hpp"

// Création des différents capteurs montés sur le robot.
MeLineFollower onBoardLineFinder(PORT_1);
MeUltrasonicSensor onBoardUltrasonicSensor(PIN_ONBOARD_ULTRASONIC_SENSOR);
MeInfraredReceiver onBoardInfraredSensor(PIN_ONBOARD_INFRARED_SENSOR);

// Création des différents actionneurs montés sur le robot.
MeDCMotor leftWheelMotor(PIN_LEFT_ONBOARD_RGB_LED);
MeDCMotor rightWheelMotor(PIN_RIGHT_ONBOARD_RGB_LED);
MeRGBLed onBoardLeftRGBLED(PIN_LEFT_ONBOARD_RGB_LED);
MeRGBLed onBoardRightRGBLED(PIN_RIGHT_ONBOARD_RGB_LED);
MeBuzzer onBoardBuzzer;

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    /*          ZONE À NE PAS MODIFIER          */
    
    // Démarrage de la communication avec l'ordinateur.
    Serial.begin(115200);

    // Définition des broches des capteurs.
    pinMode(PIN_ONBOARD_BUTTON, INPUT);

    // Lancement du capteur infrarouge (pour détecter les appuis de la télécommande).
    onBoardInfraredSensor.begin();

    /*          ZONE À NE PAS MODIFIER          */
    
    
    // Pour déplacer le robot, utiliser ceci :
    moveMBot(FORWARD, 100);

    // Pour attendre un délai en milliseconde, faire :
    delay(1000);

    // Pour arrêter le robot, mettre la vitesse à 0 :
    moveMBot(FORWARD, 0);

    // Pour faire sonner le buzzer à une fréquence pendant un temps, faire :
    // onBoardBuzzer.tone(900, 1000000);

    for (int i = 0; i < 9000; i++)
    {
        onBoardBuzzer.tone(i, 1);
    }
    for (int i = 9000; i > 0; i--)
    {
        onBoardBuzzer.tone(i, 1);
    }

    // Pour changer la couleur des DEL RVB, faire :
    setLED(255, 0, 200);
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    // Test du détecteur de ligne.
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

    // Si une touche a été pressée mais que le programme ne l'a pas encore pris en compte.
    if (onBoardInfraredSensor.available())
    {
        // On récupère la touche pressée.
        unsigned int receiverCode = onBoardInfraredSensor.read();

        // Affichage de la touche pressée.
        switch (receiverCode)
        {
        case IR_BUTTON_A:
            Serial.println("Press A.");
            break;
        case IR_BUTTON_B:
            Serial.println("Press B.");
            break;
        case IR_BUTTON_C:
            Serial.println("Press C.");
            break;
        case IR_BUTTON_D:
            Serial.println("Press D.");
            break;
        case IR_BUTTON_E:
            Serial.println("Press E.");
            break;
        case IR_BUTTON_F:
            Serial.println("Press F.");
            break;
        case IR_BUTTON_SETTING:
            Serial.println("Press Setting.");
            break;
        case IR_BUTTON_UP:
            Serial.println("Press Up.");
            break;
        case IR_BUTTON_DOWN:
            Serial.println("Press Down.");
            break;
        case IR_BUTTON_LEFT:
            Serial.println("Press Left.");
            break;
        case IR_BUTTON_RIGHT:
            Serial.println("Press Right.");
            break;
        case IR_BUTTON_0:
            Serial.println("Press 0.");
            break;
        case IR_BUTTON_1:
            Serial.println("Press 1.");
            break;
        case IR_BUTTON_2:
            Serial.println("Press 2.");
            break;
        case IR_BUTTON_3:
            Serial.println("Press 3.");
            break;
        case IR_BUTTON_4:
            Serial.println("Press 4.");
            break;
        case IR_BUTTON_5:
            Serial.println("Press 5.");
            break;
        case IR_BUTTON_6:
            Serial.println("Press 6.");
            break;
        case IR_BUTTON_7:
            Serial.println("Press 7.");
            break;
        case IR_BUTTON_8:
            Serial.println("Press 8.");
            break;
        case IR_BUTTON_9:
            Serial.println("Press 9.");
            break;
        default:
            break;
        }
    }
}