#include <Arduino.h>
#include <openmv.h>
#include <string.h>
#include "Behaviors.h"

// behavior class
Behaviors behavior;

// OpenMV camera;
//uint8_t FindAprilTags();

void setup() {
  // Lab 5 - Vision  
  //
  // Serial.begin(115200);
  // delay(1000);
  // Wire.begin();
  // Wire.setClock(100000ul);
  //
  // Serial.println(F("Welcome."));
  // behavoir.Init();

  Serial.begin(1000);
  delay(1000);  //give it a moment to bring up the Serial
  Serial.println("setup()"); 

  behavior.Init();
}

void loop() {
    behavior.Run2();
    behavior.updateMQTT();
    // behavior.test();
}

// uint8_t FindAprilTags()
// {
//     uint8_t tagCount = camera.getTagCount();
//     if(tagCount) 
//     {
//       Serial.println(tagCount);
//       AprilTagDatum tag;
//       if(camera.readTag(tag))
//       {
//         Serial.print(F("Tag [cx="));
//         Serial.print(tag.cx);
//         Serial.print(F(", cy="));
//         Serial.print(tag.cy);
//         Serial.print(F(", w="));
//         Serial.print(tag.w);
//         Serial.print(F(", h="));
//         Serial.print(tag.h);
//         Serial.print(F(", id="));
//         Serial.print(tag.id);
//         Serial.print(F(", rot="));
//         Serial.print(tag.rot);
//         Serial.println(F("]"));
//       }
//     }
//     return tagCount;
// }
