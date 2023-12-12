#include <Arduino.h>
#include <openmv.h>
#include <string.h>
#include "Behaviors.h"
#include "IR_sensor.h"
#include "Sonar_sensor.h"

// behavior class
Behaviors positionEstimation;
IRsensor irSensor;
SonarSensor sonar;
// OpenMV camera;

// uint8_t FindAprilTags();

/**
 * sendMessage creates a string of the form
 *      topic:message
 * which is what the corresponding ESP32 code expects.
 * */
static void sendMessage(const String& topic, const String& message)
{
    Serial1.println(topic + String(':') + message);
    Serial.println(topic + String(':') + message);
}

String serString1;
bool checkSerial1(void)
{
    while(Serial1.available())
    {
        char c = Serial1.read();
        serString1 += c;

        if(c == '\n')
        {
            return true;
        }
    }

    return false;
}

void setup() {
  // Lab 5 - Vision

  // Serial.begin(115200);
  // delay(1000);

  // Wire.begin();
  // Wire.setClock(100000ul);

  // Serial.println(F("Welcome."));
  // positionEstimation.Init();

  // Lab 6 MQTT

  Serial.begin(115200);
  delay(10000);  //give it a moment to bring up the Serial

  Serial.println("setup()");

  Serial1.begin(115200);
  digitalWrite(0, HIGH); // Set internal pullup on RX1 to avoid spurious signals

  Serial.println("/setup()");

  irSensor.Init();
  sonar.Init();
  positionEstimation.Init();
}

void loop() {
  // positionEstimation.Run();

  // Lab 5 - Vision
  // TODO - Make PID (probably just P loop) for distance to tag (Target is w,h == 45)
  // TODO - Make PID (probably just P) for angling to tag

   positionEstimation.Run();
  // delay(1); //calm things down for a sec...
  // // FindAprilTags();

  // Lab 6
    static uint32_t lastSend = 0;
    uint32_t currTime = millis();
    if(currTime - lastSend >= 500) //send every five seconds
    {
        lastSend = currTime;
        sendMessage("timer/time", String(currTime));
        sendMessage("irSensor/distance", String(irSensor.ReadData()));
        sendMessage("sonarSensor/distance", String(sonar.ReadData()));
    }

    // Check to see if we've received anything
    if(checkSerial1())
    {
        Serial.print("Rec'd:\t");
        Serial.print(serString1);
        serString1 = "";
    }

    // Defaults to just sending one message, but increase the message count
    // if you want to test how fast you can send
    static int msgCountToSend = 0;

    while(msgCountToSend)
    {
        sendMessage("button/time", String(currTime + msgCountToSend--));
    }
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
