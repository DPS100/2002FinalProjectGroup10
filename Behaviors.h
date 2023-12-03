#ifndef BEHAVIORS
#define BEHAVIORS

#include <Romi32U4.h>
#include <Arduino.h>
#include <openmv.h>

class Behaviors{
    private:
        enum ROBOT_STATE {IDLE, DRIVE};
        ROBOT_STATE robot_state = IDLE; //initial state: IDLE
        int TARGET_W =45, TARGET_H = 45;
        const float Kp1 = 10;
        const float Kp2 = 4;
         
    public:
        void Init(void); /// a and b
        void Stop(void);
        void Run(void);
};

#endif