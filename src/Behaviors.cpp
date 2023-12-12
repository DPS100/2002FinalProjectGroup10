#include <Romi32U4.h>
#include "Behaviors.h"
#include "Speed_controller.h"
#include "Position_estimation.h"

//sensors
Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;

//motor-speed controller
SpeedController robot;

//camera
OpenMV camera;

void Behaviors::Init(void)
{
    robot.Init();
}

void Behaviors::Stop(void)
{
    robot.Stop();
}


void Behaviors::Run2(void) {
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
        // if (collisoncode here){

        //     robot_state = BUMP;
        //     robot.Stop();
        // }
        else{
            //straight forward for now
            robot.Straight(50,5); //speed, time
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
