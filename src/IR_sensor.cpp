#include <Romi32U4.h>
#include "IR_sensor.h"

void IRsensor::Init(void)
{
    pinMode(pin_IR, INPUT);
}

float IRsensor::PrintData(void)
{
  float data = ReadData();
  Serial.println(data);
  return data;
}

float IRsensor::ReadData(void)
{
  //assignment 1.1
  //read out and calibrate your IR sensor, to convert readouts to distance in [cm]
  float voltage = (float)analogRead(pin_IR) / maxADC * maxVolt;
  
  // Bounds check
  if(voltage > volt[0]) { // Too close to read
    return dist[0]; 
  } else if(voltage <= volt[datapoints - 1]) { // Too far to read
    return dist[datapoints - 1];
  }

  // Find voltage value bounds
  int lowerDistIndex = 0;
  int upperVoltIndex = 1;
  for(int i = 1; i < datapoints; i++) {
    if(voltage >= volt[i] && voltage < volt[i - 1]) {
      lowerDistIndex = i;
      upperVoltIndex = i;
    }
  }

  float deltaDistance = dist[lowerDistIndex] - dist[lowerDistIndex - 1];
  float deltaVoltage = volt[upperVoltIndex] - volt[upperVoltIndex - 1];
  float sensitivity = deltaDistance / deltaVoltage;

  float baseDistance = dist[lowerDistIndex];
  float offsetVoltage = voltage - volt[upperVoltIndex];
  float offsetDistance = offsetVoltage * sensitivity;

  return baseDistance + offsetDistance;
}