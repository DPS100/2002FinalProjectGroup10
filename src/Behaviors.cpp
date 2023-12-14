#include <Romi32U4.h>
#include "Behaviors.h"
#include "Speed_controller.h"
#include "Position_estimation.h"

//sensors
IRsensor irSensor;
SonarSensor sonar;
OpenMV camera;

//buttons
Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;

//motor-speed controller
SpeedController robot;

Position pos;

//used for mqtt checkSerial1
String serString1;

void Behaviors::Init(void){
    //sensors
    robot.Init();
    irSensor.Init();
    sonar.Init();
    //camera
    Wire.begin();
    Wire.setClock(100000ul);

    //speeed controls
    pos.Init();

    //mqtt
    Serial1.begin(115200);
    digitalWrite(0, HIGH); // Set internal pullup on RX1 to avoid spurious signals
    Serial.println("/setup()");
}

void Behaviors::Stop(void){
    robot.Stop();
}
/*
 * sendMessage creates a string of the form
 *      topic:message
 * which is what the corresponding ESP32 code expects.
 */
void sendMessage(const String& topic, const String& message){
    Serial1.println(topic + String(':') + message);
    Serial.println(topic + String(':') + message);
}

bool checkSerial1(void){
    while(Serial1.available())
    {
        char c = Serial1.read(); //reads one char at a time
        serString1 += c; //adds char until it reaches \n

        if(c == '\n')
        {
            return true;
        }
    }
    return false;
}

void Behaviors::findTags(void){
    uint8_t tagCount = camera.getTagCount();
    // static int missed = 0;
    if(tagCount){
        AprilTagDatum tag1;
        //missed = 0;
        if(camera.readTag(tag1)){
            sendMessage("AprilTag", String(tag1.id));
            if(tag1.w < TARGET_W){
                sendMessage("Romi X",String(pos.ReadPose().X));
                sendMessage("Romi Y",String(pos.ReadPose().Y));
                sendMessage("Romi Theta",String(pos.ReadPose().THETA));
                // float errorW = TARGET_W - (int)tag1.w;
                // float errorX = 80-(int)tag1.cx;
            
                // float u_distance = Kp1 * errorW + Kd * (errorW - prevError);
                // float u_angle = Kp2 * errorX;

                // robot.Run(u_distance-u_angle, u_distance+u_angle);
                // prevError = errorW;
            }
        }
        // } else if(tag1.w >= TARGET_W){
        //     robot_state = PAYLOAD;
        //     robot.Stop();
        // }
    }
    // } else {
    //     missed++;
    //     if(missed>80){
    //         prevError = 0;
    //         robot.Run(0,0);
    //     }
    // }
}

void Behaviors::Run2(void) {
// Look around for april tags until bumps into wall
    robot_state = SEEK;
    switch (robot_state){
    case SEEK:
        delay(1);
        findTags();

    case IDLE:
        if(buttonA.getSingleDebouncedRelease()){ 
            robot_state = WANDER; 
            robot.Stop();             
        } 
        else { 
            robot_state = IDLE;
            robot.Stop(); 
        }   
        break;
    
    case WANDER:
        if (sonar.ReadData() < 5){
            sendMessage("Sonar", String("Wall Here"));
            robot_state = BUMP;
            robot.Stop();
        }
        else{
            robot.Run(50,50); //speed, time
            robot_state = WANDER;
        }
        break;

    case BUMP:
        robot.Turn(90,1); // degree, direction
        robot_state = WANDER;
        robot.Stop();
        break;

    case PAYLOAD:
        //find fastest way back
        // needs the visual map
        Serial.println("Sucess?");
        //robot_state = IDLE;
        robot.Stop();
        break;
    }
}

void Behaviors::test(void){
    if(buttonA.getSingleDebouncedRelease()){ 
        Serial.println("button pressed");
        
        float shortestDistOne = 9999999;
        float shortestDistTwo = 9999999;
        unsigned long now1 = millis();
        float curr;

        while((unsigned long)(millis() - now1) <= 3.5/2*1000){ //90 degrees
            robot.setEfforts(50,-50);
            curr = irSensor.ReadData();
            if(curr < shortestDistOne){
                shortestDistOne = curr;
                Serial.print("shortest:");   
                Serial.println(shortestDistOne);
            }
        }
        robot.setEfforts(0,0);

        unsigned long now2 = millis();
        while((unsigned long)(millis() - now2) <= 3.3*1000){
            robot.setEfforts(50,-50);
            curr = irSensor.ReadData();
            if(curr < shortestDistTwo){
                shortestDistTwo = curr;
                Serial.print("shortest:");   
                Serial.println(shortestDistTwo);
            }
        }
        robot.setEfforts(0,0);  
    }

}
    //     // now = millis();
    //     // while((unsigned long)(millis() - now) <= 3*1000){  //3sec
    //     //     robot.setEfforts(50,-50);       
    //     //     if(sonar.ReadData() < shortestDistTwo){
    //     //         shortestDistTwo = sonar.ReadData();
    //     //         Serial.println(shortestDistTwo);
    //     //     }
    //     // }
    //     robot.setEfforts(0,0);
    //     Serial.print(shortestDistOne+shortestDistTwo+21);
        //sendMessage("info/width",String(shortestDistOne+shortestDistTwo+21));
//     }      
// }