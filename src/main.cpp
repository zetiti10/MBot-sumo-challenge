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
MeLineFollower onBoardLineFinder(PIN_ONBOARD_LINE_FINDER);
MeUltrasonicSensor onBoardUltrasonicSensor(PIN_ONBOARD_ULTRASONIC_SENSOR);
MeIR onBoardInfraredSensor;

// Création des différents actionneurs montés sur le robot.
MeDCMotor leftWheelMotor(PIN_LEFT_WHEEL_MOTOR);
MeDCMotor rightWheelMotor(PIN_RIGHT_WHEEL_MOTOR);
MeRGBLed onBoardRGBLED(PIN_RIGHT_ONBOARD_RGB_LED, PIN_LEFT_ONBOARD_RGB_LED);
MeBuzzer onBoardBuzzer;

// Variables globales.
Mode mode = MANUAL;
FightMode fightMode = READY;
int speed = 100;
int rotationSpeed = 100;
int ultrasonicDistance;
unsigned long mainScheduler = 0;
unsigned long stopMoveScheduler = 0;
int rotationTime = 800;
int soundTime = 100;
int fightSpeed = 50;
int fightRotationSpeed = 50;
int aleatory = 400;
long aleaPercentage = 6;

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    // Démarrage de la communication avec l'ordinateur.
    Serial.begin(115200);

    // Définition des broches des capteurs.
    pinMode(PIN_ONBOARD_BUTTON, INPUT);

    randomSeed(analogRead(0));

    // Lancement du capteur infrarouge (pour détecter les appuis de la télécommande).
    onBoardInfraredSensor.begin();
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    // Communication aves l'ordinateur
    if (Serial.available())
    {
        // Récupération du message
        delay(UART_WAITING_TIME);

        String receivedMessage;
        boolean stop = true;
        while (Serial.available() > 0 && stop)
        {
            char letter = Serial.read();

            if (letter == '\n')
                stop = false;

            receivedMessage += letter;
        }

        // Exécution de la requête du message.
        switch (receivedMessage[0])
        {
        case '0':
            moveMBot(receivedMessage[1] - '0');
            stopMoveScheduler = millis() + (receivedMessage[2] - '0') * 100;
            while (Serial.available())
                Serial.read();

            break;

        case '1':
            moveMBot(receivedMessage[1] - '0');
            break;

        case '2':
            moveMBot(STOP);
            setLED(0, 0, 0);
            fightMode = READY;
            mode = static_cast<Mode>((receivedMessage[1] - '0') - 1);
            break;

        case '3':
            if (receivedMessage[1] == '1')
                speed = (receivedMessage[4] - '0') + (receivedMessage[3] - '0') * 10 + (receivedMessage[2] - '0') * 100;

            else if (receivedMessage[1] == '2')
                rotationSpeed = (receivedMessage[4] - '0') + (receivedMessage[3] - '0') * 10 + (receivedMessage[2] - '0') * 100;

            break;

        case '4':
        {
            int r = (receivedMessage[3] - '0') + (receivedMessage[2] - '0') * 10 + (receivedMessage[1] - '0') * 100;
            int g = (receivedMessage[6] - '0') + (receivedMessage[5] - '0') * 10 + (receivedMessage[4] - '0') * 100;
            int b = (receivedMessage[9] - '0') + (receivedMessage[8] - '0') * 10 + (receivedMessage[7] - '0') * 100;
            setLED(r, g, b);
            break;
        }

        default:
            break;
        }
    }

    // Mode autonome.
    if (mode == AUTONOMOUS)
    {
        // Gestion de la lymière en fonction de la distance mesurée.
        int LEDDistance = map(ultrasonicDistance, 0, 20, 255, 0);
        if (LEDDistance < 0)
            LEDDistance = 0;
        setLED(LEDDistance, 0, 0);

        moveMBot(FORWARD);

        // On tourne si un obstacle est détecté.
        if (ultrasonicDistance < 10)
        {
            moveMBot(BACKWARD);
            delay(300);
            moveMBot(RIGHT);
            delay(300);
        }
    }

    // Mode suiveur de ligne.
    else if (mode == LINE_FOLLOWER)
    {
        switch (onBoardLineFinder.readSensors())
        {
        case S1_IN_S2_IN:
            setLED(255, 255, 0);
            moveMBot(FORWARD);
            break;
        case S1_IN_S2_OUT:
            setLeftLED(255, 255, 0);
            setRightLED(0, 0, 0);
            moveMBot(LEFT);
            break;
        case S1_OUT_S2_IN:
            setLeftLED(0, 0, 0);
            setRightLED(255, 255, 0);
            moveMBot(RIGHT);
            break;
        case S1_OUT_S2_OUT:
            setLED(0, 0, 0);
            moveMBot(BACKWARD);
            break;
        default:
            break;
        }
    }

    // Mode autonome de combat.
    else if (mode == FIGHT)
    {
        if (fightMode == READY)
        {
            // Lancement du combat.
            if (buttonPressed())
            {
                while (buttonPressed())
                    delay(1);

                delay(10);

                fightMode = FIGHTING;

                speed = 90;
                rotationSpeed = fightRotationSpeed;

                // Délai des 5 secondes.
                setLED(100, 100, 0);
                onBoardBuzzer.tone(10000, soundTime);
                delay((1000 * 5) - (2 * soundTime));
                onBoardBuzzer.tone(10000, soundTime);

                int startMove = random(3);

                switch (startMove)
                {
                case 0:
                    moveMBot(LEFT);
                    delay(2 * rotationTime);
                    moveMBot(FORWARD);
                    break;

                case 1:
                    moveMBot(RIGHT);
                    delay(2 * rotationTime);
                    moveMBot(FORWARD);
                    break;

                case 2:
                    moveMBot(BACKWARD);
                    delay(2 * rotationTime);
                    break;
                }

                speed = fightSpeed;
                moveMBot(FORWARD);
            }
        }

        else if (fightMode == FIGHTING)
        {
            int aleatoryRotation = random((rotationTime - aleatory), (rotationTime + aleatory));

            switch (onBoardLineFinder.readSensors())
            {
            case S1_IN_S2_OUT:
                moveMBot(LEFT);
                delay(aleatoryRotation);
                moveMBot(FORWARD);
                break;
            case S1_OUT_S2_IN:
                moveMBot(RIGHT);
                delay(aleatoryRotation);
                moveMBot(FORWARD);
                break;
            case S1_OUT_S2_OUT:
                moveMBot(LEFT);
                delay(2 * aleatoryRotation);
                moveMBot(FORWARD);
                break;
            default:
                break;
            }

            if (ultrasonicDistance < 20)
            {
                speed = 100;
                moveMBot(FORWARD);
            }

            else
            {
                speed = fightSpeed;
                moveMBot(FORWARD);

                delay(1);
                long r = random(10000);

                if (r < aleaPercentage)
                {
                    delay(1);
                    int aleaMove = random(2);

                    if (aleaMove == 0)
                        moveMBot(LEFT);

                    else
                        moveMBot(RIGHT);
                        
                    delay(aleatoryRotation);
                    moveMBot(FORWARD);
                }
            }

            if (buttonPressed())
            {
                while (buttonPressed())
                    delay(1);

                delay(10);

                fightMode = READY;

                moveMBot(STOP);
                setLED(0, 0, 0);

                onBoardBuzzer.tone(10000, soundTime);
            }
        }
    }

    // Exécution des tâches périodiques.
    if (millis() - mainScheduler >= 100)
    {
        mainScheduler = millis();

        // Lecture de la valeur du capteur de distance.
        ultrasonicDistance = onBoardUltrasonicSensor.distanceCm();

        // Envoi des données au logiciel.
        String message = "0";
        if (ultrasonicDistance < 100)
        {
            if (ultrasonicDistance < 10)
                message += "0";
            message += ultrasonicDistance;
        }

        else
            message += "99";

        Serial.println(message);
    }

    // Gestion de l'arrêt programmé du robot.
    if (stopMoveScheduler != 0 && millis() > stopMoveScheduler)
    {
        stopMoveScheduler = 0;

        moveMBot(STOP);
    }
}