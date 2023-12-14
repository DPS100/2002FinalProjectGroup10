#include <Romi32U4.h>
#include "Encoders.h"
#include "Speed_controller.h"
#include "Position_estimation.h"
#include "Behaviors.h"

Romi32U4Motors motors;
Encoder MagneticEncoder; 
Position odometry;

void SpeedController::Init(void)
{
    MagneticEncoder.Init();
    odometry.Init();
}

void SpeedController::Run(float target_velocity_left, float target_velocity_right, float steer_left, float steer_right)
{
    if(MagneticEncoder.UpdateEncoderCounts()){
        float lspeed = MagneticEncoder.ReadVelocityLeft();
        float rspeed = MagneticEncoder.ReadVelocityRight();
        float e_left = target_velocity_left + steer_left - lspeed;
        float e_right = target_velocity_right + steer_right - rspeed;

        E_left += e_left;
        E_right += e_right;

        float u_left = ff*(target_velocity_left + steer_left) + Kp*e_left + Ki*E_left;
        float u_right = ff*(target_velocity_right + steer_right) + Kp*e_right + Ki*E_right;

        motors.setEfforts(u_left,u_right);
        odometry.UpdateIdealPose(target_velocity_left, target_velocity_right); //this is where your newly programmed function is/will be called
    }
}

void SpeedController::Run(float target_velocity_left, float target_velocity_right) {
    Run(target_velocity_left, target_velocity_right, 0.0f, 0.0f);
}

boolean SpeedController::Turn(int degree, int direction)
{
    motors.setEfforts(0, 0);
    int turns = ((float)degree/degreesPerRotation)*((float)counts*factor)*(((float)dr)/((float)rw)); 
    int count_turn = MagneticEncoder.ReadEncoderCountLeft();
    Position::pose_data pre_pose = odometry.ReadPose();
    float directionSign = direction ? 1 : -1;
    odometry.PrintPose();
    Position::pose_data post_pose = {pre_pose.X, pre_pose.Y, pre_pose.THETA + 2.0f * PI * degree/degreesPerRotation * directionSign};

    while(abs(abs(count_turn) - abs(MagneticEncoder.ReadEncoderCountLeft())) <= turns)
    {
        if(!direction) {
            Run(50,-50);
        } else {
            Run(-50,50);
        } 
    }
    odometry.SetPose(post_pose);
    odometry.PrintPose();
    motors.setEfforts(0, 0);
    return 1;
}

boolean SpeedController::Straight(int target_velocity, float time) //in mm/s and s
{
    motors.setEfforts(0, 0);
    unsigned long now = millis();

    while ((unsigned long)(millis() - now) <= time*1000){
        Run(target_velocity,target_velocity);
    }
    motors.setEfforts(0, 0);
    return 1;
}

boolean SpeedController::Curved(int target_velocity_left, int target_velocity_right, int time) //in mm/s and s
{
    motors.setEfforts(0, 0);
    
    unsigned long now = millis();

    while ((unsigned long)(millis() - now) <= time*1000){
        Run(target_velocity_left,target_velocity_right);
    }
    motors.setEfforts(0, 0);
    return 1;
}

void SpeedController::Stop()
{
    motors.setEfforts(0,0);
    odometry.Stop();
}

void SpeedController::setEfforts(float l, float r){
    motors.setEfforts(l,r);
    odometry.UpdateIdealPose(MagneticEncoder.ReadVelocityLeft(),MagneticEncoder.ReadVelocityRight());
}

Position::pose_data SpeedController::readPose(void) {
    return odometry.ReadPose();
}
