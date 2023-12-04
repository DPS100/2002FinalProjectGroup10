#ifndef POSITION_ESTIMATION
#define POSITION_ESTIMATION

#include <Romi32U4.h>

class Position{
    private:
        float x, y, theta = 0;
        float trueX, trueY, trueTheta = 0;
        float deltaT;
        
        unsigned long time_prev, time_now = 0;
        const float l = 142.875;//142.875; //assignment (mm)
        
    public:
        struct pose_data {
            float X;
            float Y;
            float THETA;
        };
        void Init(void);
        void UpdateIdealPose(float,float,float,float);
        pose_data ReadPose(void);
        void PrintPose(void);
        void Stop(void);
};

#endif