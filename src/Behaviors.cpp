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

void Behaviors::Run(void)
{
    switch (robot_state)
    {
    case IDLE:
        if(buttonA.getSingleDebouncedRelease()){ 
            robot_state = DRIVE; 
            robot.Stop();             
        } 
        else { 
            robot_state = IDLE;
            robot.Stop(); 
        }   
        break;
    
    case DRIVE:
        if (buttonB.getSingleDebouncedRelease()) {
            robot_state = IDLE;
            break;
        }
        // Use SpeedController::Run once efforts have been determined
        // The target distance fr April tags is W = 45, H = 45
        
        // Maybe use area as a target?
        uint8_t tagCount = camera.getTagCount();
        if(tagCount) 
        {
            AprilTagDatum tag;
            if(camera.readTag(tag))
            {
                float errorW = TARGET_W - tag.w;
                float u_left = Kp* errorW;
                Serial.println(u_left);

                robot.Run(u_left, u_left);
            }
        }
        break;
    }
}
