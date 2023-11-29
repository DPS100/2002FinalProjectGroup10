#include <Arduino.h>
#include <Wire.h>
#include <openmv.h>

OpenMV camera;

void loop() 
{ 
  delay(1); //calm things down for a sec...
  FindAprilTags();
}
