#ifndef BEHAVIORS
#define BEHAVIORS

#include <Romi32U4.h>
#include <Arduino.h>
#include <openmv.h>

class Behaviors{
    private:
        enum ROBOT_STATE {IDLE, DRIVE};
        ROBOT_STATE robot_state = IDLE; //initial state: IDLE
        unsigned int TARGET_W, TARGET_H = 45;
        const float Kp = 0.0005;
         
    public:
        void Init(void); /// a and b
        void Stop(void);
        void Run(void);
};

#endif