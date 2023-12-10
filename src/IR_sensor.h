#ifndef IR_SENSOR
#define IR_SENSOR

#include <Romi32U4.h>

class IRsensor{
    private:
        const int pin_IR = A0;
        static const int datapoints = 11;
        const float dist[datapoints] = {6.0f, 7.0f, 8.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 40.0f, 50.0f, 80.0f};
        const float volt[datapoints] = {3.15f, 3.0f, 2.75f, 2.3f, 1.65f, 1.3f, 1.1f, 0.9f, 0.75f, 0.6f, 0.4f};
        const float maxADC = 1023.0f;
        const float maxVolt = 5.0f;


        const float min_dist = 10.0f; // cm
        const float min_dist_v = 2.3f; 
        const float max_dist = 80.0f; // cm
    public:
        void Init(void);
        float ReadData(void);
        float PrintData(void);
};

#endif