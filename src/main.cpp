#include <Arduino.h>
#include <openmv.h>
#include "Behaviors.h"

// behavior class
Behaviors positionEstimation;
// OpenMV camera;

// uint8_t FindAprilTags();

void setup() {
  // positionEstimation.Init();

  // Lab 5 - Vision

  Serial.begin(115200);
  delay(1000);

  Wire.begin();
  Wire.setClock(100000ul);

  Serial.println(F("Welcome."));
  positionEstimation.Init();
}

void loop() {
  // positionEstimation.Run();

  // Lab 5 - Vision
  // TODO - Make PID (probably just P loop) for distance to tag (Target is w,h == 45)
  // TODO - Make PID (probably just P) for angling to tag
  positionEstimation.Run();
  delay(1); //calm things down for a sec...
  // FindAprilTags();
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