#include <Romi32U4.h>
#include "Sonar_sensor.h"

void SonarSensor::Init(void)
{
    pinMode(pin_TRIG,OUTPUT);
    pinMode(pin_ECHO, INPUT);
    dist[0] = 1000.0f;
    dist[1] = 1000.0f;
    dist[2] = 1000.0f;
    dist[3] = 1000.0f;
    dist[4] = 1000.0f;
}

float SonarSensor::PrintData(void)
{
    float data = ReadData();
    return data;
}

float SonarSensor::ReadData(void)
{
    //assignment 1.2
    //read out and calibrate your sonar sensor, to convert readouts to distance in [cm]
    // Running average

    digitalWrite(pin_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_TRIG, LOW);
    float durationmicro = pulseIn(pin_ECHO, HIGH);
    float duration = durationmicro / MICRO_TO_SECOND;
    float distance = duration * SPEED_OF_SOUND / 2.0f * 100.0f; //cm
    dist[index++ % 5] = distance;
    float avg = 0.0f;
    for(int i = 0; i < 5; i++) {
        avg += dist[i];
    }
    return avg / 5.0f;
}