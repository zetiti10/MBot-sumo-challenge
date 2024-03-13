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
#include "pitches.h"

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
int policeCounter = 0;
boolean policeMode = false;

int rotationTime = 300;
int soundTime = 100;
int fightSpeed = 75;
int fightRotationSpeed = 75;
int fightAttackSpeed = 100;
int aleatory = 300;
long aleaPercentage = 6;
unsigned long rotationTimer = 0;
unsigned long aleaTimer = 0;
unsigned long attachedTimer = 0;
int alternate = LEFT;

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

void push()
{
    moveMBot(BACKWARD);
    delay(300);
    moveMBot(FORWARD);
    delay(500);
}

void startFight()
{
    while (buttonPressed())
        delay(1);

    delay(10);

    fightMode = FIGHTING;
    mode = FIGHT;

    speed = 90;
    rotationSpeed = fightRotationSpeed;

    // Délai des 5 secondes.
    onBoardBuzzer.tone(10000, soundTime);
    delay((4500) - (2 * soundTime));
    onBoardBuzzer.tone(10000, soundTime);
    rotationTimer = millis();
    aleaTimer = millis();
    attachedTimer = millis();
}

void stopFight()
{
    while (buttonPressed())
        delay(1);

    delay(10);

    fightMode = READY;

    moveMBot(STOP);

    onBoardBuzzer.tone(10000, soundTime);
}

void playFrequences()
{
    for (int i = 0; i < 20000; i += 15)
    {
        onBoardBuzzer.tone(i, 1);
    }
}

void schedulePolice(int frequency)
{
    onBoardBuzzer.tone(frequency, SIREN_SPEED_2);

    policeCounter++;

    if (policeCounter >= LIGHT_SPEED)
    {
        policeCounter = 0;

        if (policeMode)
        {
            setLeftLED(255, 0, 0);
            setRightLED(0, 0, 255);
            moveMBot(RIGHT);
        }

        else
        {
            setLeftLED(0, 0, 255);
            setRightLED(255, 0, 0);
            moveMBot(LEFT);
        }

        policeMode = !policeMode;
        moveMBot(STOP);
    }
}

void police()
{
    for (int i = 0; i < 20; i++)
    {
        for (int f = 635; f <= 912; f += SIREN_SPEED_1)
            schedulePolice(f);

        for (int f = 911; f >= 634; f -= SIREN_SPEED_1)
            schedulePolice(f);
    }
}

int melody[] = {
    NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5,
    NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5,
    NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,

    NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5,
    NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5,
    NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5};

int durations[] = {
    16, 16, 16, 16,
    32, 16, 8, 16,
    16, 16, 16, 32, 16, 8,

    16, 16, 16, 16, 32,
    16, 8, 32, 32, 32,
    32, 32, 32, 32, 32, 16, 8};

void playVictory()
{
    int size = sizeof(durations) / sizeof(int);

    for (int note = 0; note < size; note++)
    {
        // to calculate the note duration, take one second divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int duration = 1000 / durations[note];
        onBoardBuzzer.tone(melody[note], duration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = duration * 1.30;
        delay(pauseBetweenNotes);

        // stop the tone playing:
        onBoardBuzzer.noTone();
    }
}

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    attachedTimer = millis();

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

        case '5':
            if (receivedMessage[1] == '1')
                playFrequences();

            else if (receivedMessage[1] == '2')
                police();

            else if (receivedMessage[1] == '3')
                playVictory();

        case '6':
        {
            if (receivedMessage[1] == '1')
                startFight();

            else if (receivedMessage[1] == '0')
                stopFight();

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
                startFight();
        }

        else if (fightMode == FIGHTING)
        {
            if (ultrasonicDistance < 50)
            {
                rotationTimer = millis();
                aleaTimer = millis();

                if (alternate == LEFT)
                    alternate = RIGHT;

                else
                    alternate = LEFT;

                switch (onBoardLineFinder.readSensors())
                {
                case S1_IN_S2_OUT:
                    speed = rotationSpeed;
                    moveMBot(LEFT);
                    break;
                case S1_OUT_S2_IN:
                    speed = rotationSpeed;
                    moveMBot(RIGHT);
                    break;
                case S1_OUT_S2_OUT:
                    speed = rotationSpeed;
                    moveMBot(LEFT);
                    break;
                default:
                    speed = fightAttackSpeed;
                    moveMBot(FORWARD);
                    if ((millis() - attachedTimer) > 3000)
                        push();
                    break;
                }
            }

            else
            {
                attachedTimer = millis();

                if ((millis() - rotationTimer) > 4000)
                {
                    int aleatoryRotation = random((rotationTime - aleatory), (rotationTime + aleatory));

                    switch (onBoardLineFinder.readSensors())
                    {
                    case S1_IN_S2_OUT:
                        speed = rotationSpeed;
                        moveMBot(LEFT);
                        delay(aleatoryRotation);
                        speed = fightSpeed;
                        moveMBot(FORWARD);
                        break;
                    case S1_OUT_S2_IN:
                        speed = rotationSpeed;
                        moveMBot(RIGHT);
                        delay(aleatoryRotation);
                        speed = fightSpeed;
                        moveMBot(FORWARD);
                        break;
                    case S1_OUT_S2_OUT:
                        speed = rotationSpeed;
                        moveMBot(LEFT);
                        delay(2 * aleatoryRotation);
                        speed = fightSpeed;
                        moveMBot(FORWARD);
                        break;
                    default:
                        speed = fightSpeed;
                        moveMBot(FORWARD);
                        break;
                    }

                    if ((millis() - aleaTimer) > 10000)
                        rotationTimer = millis();
                }

                else
                {
                    speed = fightRotationSpeed;
                    moveMBot(alternate);
                    aleaTimer = millis();
                }
            }

            if (buttonPressed())
            {
                stopFight();
            }
        }
    }

    // Exécution des tâches périodiques.
    if (millis() - mainScheduler >= 50)
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