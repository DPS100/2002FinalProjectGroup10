#ifndef BEHAVIORS
#define BEHAVIORS

#include <Romi32U4.h>
#include <Arduino.h>
#include <openmv.h>

//senor include statements
#include "IR_sensor.h"
#include "Sonar_sensor.h"
#include "Speed_controller.h"
#include "Position_estimation.h"

class Behaviors{
    private:
        enum ROBOT_STATE {IDLE, WANDER, BUMP, PAYLOAD,SEEK};
        ROBOT_STATE robot_state = IDLE; //initial state: IDLE
        int TARGET_W =70, TARGET_H = 45;
        float prevError = 0;
        const float Kp1 = 20;
        const float Kp2 = 4;
        const float Kd = .7;

    public:
        void Init(void); /// a and b
        void Stop(void);
        void Run(void);
        void Run2(void);
        static void updateMQTT(void); //updating mqtt
        void test(void);
        void findTags(void); //reads april tags
};

#endif