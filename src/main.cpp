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
int mode = 1;
int speed = 100;
int rotationSpeed = 100;
int ultrasonicDistance;
unsigned long mainScheduler = 0;
unsigned long stopMoveScheduler = 0;
int policeCounter = 0;
boolean policeMode = false;

// Cette fonction s'exécute une fois au démarrage du MBot.
void setup()
{
    // NE PAS SUPPRIMER LA LIGNE CI-DESSOUS !
    initialization();

    // Programme exécuté une fois au démarrage du robot.
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

// Cette fonction s'exécute en boucle après le `setup()`.
void loop()
{
    // Programme exécuté en boucle.
    if (Serial.available())
    {
        delay(UART_WAITING_TIME);

        String receivedMessage;
        boolean cont = true;
        while (Serial.available() > 0 && cont)
        {
            char letter = Serial.read();

            if(letter == '\n')
                cont = false;

            receivedMessage += letter;
        }

        switch (receivedMessage[0])
        {
        case '0':
            moveMBot(receivedMessage[1] - '0');
            stopMoveScheduler = millis() + (receivedMessage[2] - '0') * 1000;
            while (Serial.available())
                Serial.read();
            
            break;

        case '1':
            moveMBot(receivedMessage[1] - '0');
            break;

        case '2':
            mode = (receivedMessage[1] - '0');
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

            break;

        default:
            break;
        }
    }

    else if (mode == AUTO_MODE)
    {
        int LEDDistance = map(ultrasonicDistance, 0, 20, 255, 0);
        if (LEDDistance < 0)
            LEDDistance = 0;
        setLED(0, LEDDistance, 0);

        moveMBot(FORWARD);
        if (ultrasonicDistance < 10)
        {
            moveMBot(BACKWARD);
            delay(300);
            moveMBot(RIGHT);
            delay(300);
        }
    }

    else if (mode == LINE_MODE)
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

    if (millis() - mainScheduler >= 100)
    {
        mainScheduler = millis();

        ultrasonicDistance = onBoardUltrasonicSensor.distanceCm();

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

    if (stopMoveScheduler != 0 && millis() > stopMoveScheduler)
    {
        stopMoveScheduler = 0;

        moveMBot(STOP);
    }

    if (buttonPressed())
    {
        while (buttonPressed())
            delay(1);

        delay(50);

        Serial.println("1");
    }
}