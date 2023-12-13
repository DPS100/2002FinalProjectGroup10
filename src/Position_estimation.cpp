#include  "Position_estimation.h"
#include "Encoders.h"
#include <math.h>

Encoder RomiEncoders;
float x = 0;
float y = 0;
float theta = 0;
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
    x = 0; 
    y = 0;
    theta = 0;
}

Position::pose_data Position::ReadPose(void)
{
    // TODO maybe use true pos
    return {x,y,theta};
}

void Position::PrintPose(void)
{
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.print(',');
    Serial.print(theta);
    Serial.print(',');
    Serial.print(trueX);
    Serial.print(',');
    Serial.print(trueY);
    Serial.print(',');
    Serial.print(trueTheta);
    Serial.print(',');
    Serial.println(millis() / 1000.0f);
}

void Position::UpdateIdealPose(float target_speed_left, float target_speed_right, float speed_left, float speed_right)
{
    time_now = millis();
    if(time_prev == 0) {
        time_prev = time_now;
    }
    float deltaT = time_now - time_prev;
    if(deltaT >= 50) { //update every 50ms for practical reasons
        deltaT /= 1000.0f; // Milliseconds to seconds
        if(target_speed_left == target_speed_right) { // Straight          
            x += target_speed_left * cos(theta) * deltaT;
            y += target_speed_left * sin(theta) * deltaT;
            trueX += speed_left * cos(trueTheta) * deltaT;
            trueY += speed_left * sin(trueTheta) * deltaT;
        } else { // Curved
            float omega = (target_speed_right - target_speed_left) / l;
            float trueOmega = (speed_right - speed_left) / l;
            float r = (l / 2) * ((target_speed_right + target_speed_left) / (target_speed_right - target_speed_left));
            float trueR = (l / 2) * ((speed_right + speed_left) / (speed_right - speed_left));
            x += r * (sin(theta + omega * deltaT) - sin(theta));
            y += r * (- cos(theta + omega * deltaT) + cos(theta));
            trueX += trueR * (sin(trueTheta + trueOmega * deltaT) - sin(trueTheta));
            trueY += trueR * (- cos(trueTheta + trueOmega * deltaT) + cos(trueTheta));
            theta += omega * deltaT;
            trueTheta += trueOmega * deltaT;
        }
    }
    time_prev = time_now; //updating time
}

