#ifndef BEHAVIORS
#define BEHAVIORS

#include <Romi32U4.h>
#include <Arduino.h>
#include <openmv.h>

class Behaviors{
    private:
        enum ROBOT_STATE {IDLE, WANDER, BUMP, PAYLOAD};
        ROBOT_STATE robot_state = IDLE; //initial state: IDLE
        int TARGET_W =30, TARGET_H = 45;
        float prevError = 0;
        const float Kp1 = 20;
        const float Kp2 = 4;
        const float Kd = .7;

    public:
        void Init(void); /// a and b
        void Stop(void);
        void Run(void);
        void Run2(void);
};

#endif