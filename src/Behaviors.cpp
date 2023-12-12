#include <Romi32U4.h>
#include "Behaviors.h"

//sensors
IRsensor irSensor;
SonarSensor sonar;
OpenMV camera;

//buttons
Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;

//motor-speed controller
SpeedController robot;

//used for mqtt checkSerial1
String serString1; 

void Behaviors::Init(void){
    //sensors
    robot.Init();
    irSensor.Init();
    sonar.Init();

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

void Behaviors::updateMQTT(void){
    static uint32_t lastSend = 0; //time
    uint32_t currTime = millis();  
    if(currTime - lastSend >= 500) //send every five seconds 
    {
        lastSend = currTime; //updates time
        sendMessage("timer/time", String(currTime)); //print time to mqtt
        sendMessage("irSensor/distance", String(irSensor.ReadData())); //print ir data to mqtt
        sendMessage("sonarSensor/distance", String(sonar.ReadData())); //print sonar data to mqtt
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

void Behaviors::Run2(void) {
    // Look around for april tags until bumps into wall
    uint8_t tagCount = camera.getTagCount();
    //static int missed = 0;
    if(tagCount){
        AprilTagDatum tag;
        //missed = 0;
        if(camera.readTag(tag) && tag.id == 4){
            
            //record tag location
            //center camera on tag?
            robot_state = PAYLOAD;
            robot.Stop();
        }
    }
    switch (robot_state){
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
        if (false){

            robot_state = BUMP;
            robot.Stop();
        }
        else{
            //straight forward for now
            robot.Run(50,50); //speed, time
            robot_state = WANDER;
        }
        break;

    case BUMP:
        // record there is a wall there 
        // visual map stuff here
        // move back and turn
        robot.Straight(-30,5); //speed, time
        robot.Turn(90,1); // degree, direction
        robot_state = WANDER;
        robot.Stop();
        break;

    case PAYLOAD:
        //find fastest way back
        // needs the visual map
        robot_state = IDLE;
        robot.Stop();
        break;
    }
}


// void Behaviors::Run(void) {
//     robot_state = DRIVE;
//     switch (robot_state){
//     case IDLE:
//         if(buttonA.getSingleDebouncedRelease()){ 
//             robot_state = DRIVE; 
//             robot.Stop();             
//         } 
//         else { 
//             robot_state = IDLE;
//             robot.Stop(); 
//         }   
//         break;
//     case DRIVE:
//         robot.Curved(150, 180, 30);
//         robot.Curved(180, 150, 30);
//         break;
//         // if (buttonB.getSingleDebouncedRelease()) {
//         //     robot_state = IDLE;
//         //     break;
//         // }
//         // // Use SpeedController::Run once efforts have been determined
//         // // The target distance fr April tags is W = 45, H = 45
//         // Maybe use area as a target?
//         uint8_t tagCount = camera.getTagCount();
//         static int missed = 0;
//         if(tagCount){
//             AprilTagDatum tag;
//             missed = 0;
//             if(camera.readTag(tag) && tag.id == 2){
//                 //Serial.println("sees Tag");
//                 float errorW = TARGET_W - (int)tag.w;
//                 float errorX = 80-(int)tag.cx;
//             }
//         }
//         //         float u_distance = Kp1 * errorW + Kd * (errorW - prevError);
//         //         float u_angle = Kp2 * errorX;
//         //         //Serial.println(errorX);
//         //         //Serial.println(u_distance);
//         //         //Serial.println(u_angle);
//         //         robot.Run(u_distance-u_angle, u_distance+u_angle);
//         //         prevError = errorW;
//         //     }
//         // }else{
//         //     missed++;
//         //     if(missed>80){
//         //         prevError = 0;
//         //         //Serial.println("missed tagret");
//         //         robot.Run(0,0);
//         //     }
//         // }
//         break;
//     }
// }
