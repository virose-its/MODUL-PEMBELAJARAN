#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>


#define TIME_STEP 64

namespace wb = webots;

int main(int argc, char const *argv[])
{
    wb::Robot* robot = new wb::Robot();
    wb::DistanceSensor* ds[2];
    wb::Motor* wheels[4];

    // set ds
    const char* dsNames[2] = {
        "ds_right", 
        "ds_left"
    };
    for (int i = 0; i < 2; i++)
    {
        ds[i] = robot->getDistanceSensor(dsNames[i]);
        ds[i]->enable(TIME_STEP);   
    }

    // set motor
    const char* wheelsName[4] = {
        "wheel1", 
        "wheel2", 
        "wheel3", 
        "wheel4"
    };
    for (int i = 0; i < 4; i++)
    {
        wheels[i] = robot->getMotor(wheelsName[i]);
        wheels[i]->setPosition(INFINITY);
        wheels[i]->setVelocity(0.0);
    }

    int avoidObstacleCounter = 0;
    while (robot->step(TIME_STEP) != -1)
    {
        double leftSpeed = 1.0;
        double rightSpeed = 1.0;

        if (avoidObstacleCounter > 0)
        {
            avoidObstacleCounter--;
            leftSpeed = 1.0;
            rightSpeed = -1.0;
        }else
        {
            for (int i = 0; i < 2; i++)
            {
                if (ds[i]->getValue() < 950.0)
                    avoidObstacleCounter = 100;
            }
        }
        wheels[0]->setVelocity(leftSpeed);
        wheels[1]->setVelocity(rightSpeed);
        wheels[2]->setVelocity(leftSpeed);
        wheels[3]->setVelocity(rightSpeed);
    }
    
    delete robot;
    return 0;
}
