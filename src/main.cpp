//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#define INTERVALO_LEITURA 30
#define PIN_TRIGGER 5
#define PIN_ECHO 18
#define MOTOR_IN1 12
#define MOTOR_IN2 13
#define MOTOR_IN3 27
#define MOTOR_IN4 26

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BluetoothSerial.h>
#include "Ultrasonic.h"
#include "ESP32MotorControl.h"    // https://github.com/JoaoLopesF/ESP32MotorControl
#include "DifferentialSteering.h" // https://www.impulseadventure.com/elec/robot-differential-steering.html

const char auth[] = "";
const char ssid[] = "";
const char pass[] = "";

const int fPivYLimit = 32;
DifferentialSteering DiffSteer(fPivYLimit);
ESP32MotorControl MotorControl = ESP32MotorControl();
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);
BluetoothSerial SerialBT;

BLYNK_WRITE(V1)
{
    int x = param[0].asInt();
    int y = param[1].asInt();
    DiffSteer.computeMotors(x, y);
    int rightMotor = DiffSteer.computedRightMotor();
    int leftMotor = DiffSteer.computedLeftMotor();

    if (rightMotor > 0 &&  leftMotor > 0)
    {
        MotorControl.motorForward(0, rightMotor);
        MotorControl.motorForward(1, leftMotor);
    }
    else if (rightMotor < 0 && leftMotor < 0)
    {
        rightMotor = map(rightMotor, -255, 1, 255, 1);
        leftMotor = map(leftMotor, -255, 1, 255, 1);
        MotorControl.motorReverse(0, rightMotor);
        MotorControl.motorReverse(1, leftMotor);
    }
    else if (rightMotor > 0 && leftMotor < 0)
    {
        leftMotor = map(leftMotor, -255, 1, 255, 1);
        MotorControl.motorForward(0, rightMotor);
        //MotorControl.motorReverse(1, leftMotor);
    }
    else if (rightMotor < 0 && leftMotor > 00)
    {
        rightMotor = map(rightMotor, -255, 1, 255, 1);
        //MotorControl.motorReverse(0, rightMotor);
        MotorControl.motorForward(1, leftMotor);
    }
    else
        MotorControl.motorsStop();
}

void setup()
{
    //Serial.begin(115200);
    SerialBT.begin("Palito de picole");
    MotorControl.attachMotors(MOTOR_IN1, MOTOR_IN2, MOTOR_IN3, MOTOR_IN4);
    //Blynk.begin(auth, ssid, pass);
}

char data;
void loop()
{
    if (SerialBT.available()>0)
        //SerialBT.write(Serial.read());
        data = SerialBT.read();//store received data to variable
        delay(2);

        if(data == 'F'){
            MotorControl.motorForward(0, 255);
            MotorControl.motorForward(1, 220);
        }
        if(data == 'B'){
            MotorControl.motorReverse(0, 255);
            MotorControl.motorReverse(1, 220);
        }
        if(data == 'L'){
            MotorControl.motorForward(0, 255);
        }
        if(data == 'R'){
            MotorControl.motorForward(1, 220);
        }
        if(data == 'G'){
            MotorControl.motorForward(0, 255);
            MotorControl.motorForward(1, 50);            
        }
        if(data == 'I'){
            MotorControl.motorForward(0, 75);
            MotorControl.motorForward(1, 255);            
        }
        if(data == 'H'){
            MotorControl.motorReverse(0, 255);
            MotorControl.motorReverse(1, 50);            
        }
        if(data == 'J'){
            MotorControl.motorReverse(0, 75);
            MotorControl.motorReverse(1, 255);            
        }
        if(data == 'S'){
            MotorControl.motorsStop();
        }

    //motorTest();
    //Blynk.run();
}
