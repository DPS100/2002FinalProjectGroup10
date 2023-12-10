#ifndef SONAR_SENSOR
#define SONAR_SENSOR

#include <Romi32U4.h>

class SonarSensor{
    private:
        const int pin_TRIG = 5;
        const int pin_ECHO = 12;
        const float SPEED_OF_SOUND = 340.0f; // m/s
        const float MICRO_TO_SECOND = 1000000.0f;
    public:
        void Init(void); 
        float ReadData(void); 
        float PrintData(void);
};

#endif