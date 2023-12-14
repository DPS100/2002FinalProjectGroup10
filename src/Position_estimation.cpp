#include  "Position_estimation.h"
#include "Encoders.h"
#include <math.h>

Encoder RomiEncoders;
unsigned long time_prev = millis();
unsigned long time_now = 0;

void Position::Init(void)
{
    time_prev = millis();
    x = 0;
    y = 0;
    theta = 0;
}

void Position::Stop(void)
{
    time_prev = millis();
}

Position::pose_data Position::ReadPose(void)
{
    return {x,y,theta};
}
// void Position::TargetPose(targetx, targety,targettheta)
// {

// }

void Position::PrintPose(void)
{
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.print(',');
    Serial.print(theta);
    Serial.println();
}

void Position::UpdateIdealPose(float speed_left, float speed_right)
{
    time_now = millis();
    if(time_prev == 0) {
        time_prev = time_now;
    }
    float deltaT = time_now - time_prev;
    if(deltaT >= 50) { //update every 50ms for practical reasons
        deltaT /= 1000.0f; // Milliseconds to seconds
        if(speed_left == speed_right) { // Straight          
            x += speed_left * cos(theta) * deltaT;
            y += speed_left * sin(theta) * deltaT;
        } else { // Curved
            float omega = (speed_right - speed_left) / l;
            float r = (l / 2) * ((speed_right + speed_left) / (speed_right - speed_left));
            x += r * (sin(theta + omega * deltaT) - sin(theta));
            y += r * (- cos(theta + omega * deltaT) + cos(theta));
            theta += omega * deltaT;
        }
    }
    time_prev = time_now; //updating time
}

