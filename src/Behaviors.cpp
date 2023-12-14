#include <Romi32U4.h>
#include <math.h>
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

//used for mqtt checkSerial1
String serString1;

/*
 * sendMessage creates a string of the form
 *      topic:message
 * which is what the corresponding ESP32 code expects.
 */
void sendMessage(const String& topic, const String& message){
    Serial1.println(topic + String(':') + message);
    // Serial.println(topic + String(':') + message);
}

void Behaviors::Init(void){
    //sensors
    robot.Init();
    irSensor.Init();
    sonar.Init();
    
    //mqtt
    Serial1.begin(115200);
    digitalWrite(0, HIGH); // Set internal pullup on RX1 to avoid spurious signals
    Serial.println("/setup()");
    sendMessage("reset", "reset");
}

void Behaviors::Stop(void){
    robot.Stop();
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

float xPosFromOffset(float x_pos, float heading, float dist) {
    return x_pos + dist * cos(heading);
}

float yPosFromOffset(float y_pos, float heading, float dist) {
    return y_pos + dist * sin(heading);
}

void Behaviors::updateMQTT(void){
    static uint32_t lastSend = 0; //time
    uint32_t currTime = millis();  
    if(currTime - lastSend >= 250) //send every 500 ms
    {
        lastSend = currTime; //updates time
        // sendMessage("timer/time", String(currTime)); //print time to mqtt
        // sendMessage("ir/distance", String(irSensor.ReadData())); //print ir data to mqtt

        // sendMessage("sonar/distance", String(sonar.ReadData())); //print sonar data to mqtt
        Position::pose_data pose = robot.readPose();
        float sonarDist = sonar.ReadData() + 9.0f;
        float irDist = irSensor.ReadData() + 10.0f;
        float wallX = xPosFromOffset(robot.readPose().X / 10.0f, pose.THETA, sonarDist);
        float wallY = yPosFromOffset(robot.readPose().Y / 10.0f, pose.THETA, sonarDist);
        float wall2X = xPosFromOffset(robot.readPose().X / 10.0f, pose.THETA - PI/4, irDist);
        float wall2Y = yPosFromOffset(robot.readPose().Y / 10.0f, pose.THETA - PI/4, irDist);
        sendMessage("wall/x", String(wallX)); //print sonar data to mqtt
        sendMessage("wall/y", String(wallY)); //print sonar data to mqtt
        sendMessage("wall2/x", String(wall2X)); //print sonar data to mqtt
        sendMessage("wall2/y", String(wall2Y)); //print sonar data to mqtt
        sendMessage("position/x", String(pose.X / 10.0f)); //print sonar data to mqtt
        sendMessage("position/y", String(pose.Y / 10.0f)); //print sonar data to mqtt
        sendMessage("position/theta", String(pose.THETA)); //print sonar data to mqtt
    }

    // Check to see if we've received anything
    if(checkSerial1())
    {
        // Serial.print("Rec'd:\t");
        // Serial.print(serString1);
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
    // uint8_t tagCount = camera.getTagCount();
    // static int missed = 0;
    // if(tagCount){
    //     AprilTagDatum tag;
    //     missed = 0;
    //     if(camera.readTag(tag) && tag.id == 4){
    //         if(tag.w < TARGET_W){
    //             float errorW = TARGET_W - (int)tag.w;
    //             float errorX = 80-(int)tag.cx;
            
    //             float u_distance = Kp1 * errorW + Kd * (errorW - prevError);
    //             float u_angle = Kp2 * errorX;

    //             robot.Run(u_distance-u_angle, u_distance+u_angle);
    //             prevError = errorW;
    //             }
    //         }
    //         else if(tag.w >= TARGET_W){
    //             sendMessage("AprilTag", String("Found AprilTag"));
    //             robot_state = PAYLOAD;
    //             robot.Stop();
    //         }
    //     }else{
    //         missed++;
    //         if(missed>80){
    //             prevError = 0;
    //             robot.Run(0,0);
    //     }
    // }
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
        if (sonar.ReadData() < 7.5f){
            robot_state = BUMP;
            robot.Stop();
        }
        else{
            robot.Run(50,50); //speed, time
            robot_state = WANDER;
        }
        break;

    case BUMP:
        robot.Stop();
        delay(500);
        robot.Turn(90,1); // degree, direction
        robot_state = WANDER;
        robot.Stop();
        delay(500);
        break;

    case PAYLOAD:
        //if(position.ReadPose() != )
        robot_state = PAYLOAD;
        robot.Stop();
        break;
    }
}

void Behaviors::test(void){
    if(buttonA.getSingleDebouncedRelease()){ 
        Serial.println("button pressed");
        
        float shortestDistOne = 99999;
        float shortestDistTwo = 99999;

        unsigned long now = millis();

        while((unsigned long)(millis() - now) <= 3*1000){  //3sec
            robot.setEfforts(50,-50);

            if(sonar.ReadData() < shortestDistOne){
                shortestDistOne = sonar.ReadData();
                Serial.println(shortestDistOne);
            }
        }
        robot.setEfforts(0,0);

        now = millis();
        while((unsigned long)(millis() - now) <= 3*1000){  //3sec
            robot.setEfforts(40,-40);
            
            if(sonar.ReadData() < shortestDistTwo){
                shortestDistTwo = sonar.ReadData();
                Serial.println(shortestDistTwo);
            }
        }
        robot.setEfforts(0,0);
        Serial.print(shortestDistOne+shortestDistTwo+19);
        sendMessage("info/width",String(shortestDistOne+shortestDistTwo+19));
    }
}