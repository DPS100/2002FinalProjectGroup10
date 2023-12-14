#ifndef SPEED_CONTROLLER
#define SPEED_CONTROLLER

#include <Romi32U4.h>
#include "Position_estimation.h"

class SpeedController{
    private:
        const float ff = 0.7;
        const float Kp = 0.5; //Adapt the parameters until your robot moves at the speed you command it to drive
        const float Ki = 0.02; // Use 0.5 and 0.1 for reverse/turning
        float E_left = 0; 
        float E_right = 0;
        int counts = 1440; //assignment 1: convert degrees into counts
        float factor = 1.0f/2.0f;
        int dr = 140; // Wheel base in mm
        int rw = 35; // Wheel radius in mm
        float degreesPerRotation = 360.0f;

    public:
        void Init(void);
        void Run(float, float); //speed left, speed right 
        void Run(float, float, float, float); //speed left, speed right , steer left, steer right
        boolean Turn(int,int); //degrees, direction of rotation: 0->left, 1->right
        boolean Straight(int, float); //speed, duration
        boolean Curved(int,int,int); //speed left, speed right, duration
        void Stop(void);
        void setEfforts(float, float);
        Position::pose_data readPose(void);
};

#endif