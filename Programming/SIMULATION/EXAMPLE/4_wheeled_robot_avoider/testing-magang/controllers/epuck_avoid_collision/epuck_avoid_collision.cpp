#include <webots/DistanceSensor.hpp>
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
// #include <webots/robot.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28

namespace wb = webots;

int main(int argc, char** argv)
{
    wb::Robot* rob = new wb::Robot();

    wb::DistanceSensor *ps[8];
    char psNames[8][4] = {
      "ps0", "ps1", "ps2", "ps3",
      "ps4", "ps5", "ps6", "ps7"
    };

    for (int i = 0; i < 8; i++)
    {
        ps[i] = rob->getDistanceSensor(psNames[i]);
        ps[i]->enable(TIME_STEP);
    }

    wb::Motor* leftMotor = rob->getMotor("left wheel motor");
    wb::Motor* rightMotor = rob->getMotor("right wheel motor");

    leftMotor->setPosition(INFINITY);
    rightMotor->setPosition(INFINITY);
    leftMotor->setVelocity(0.0);
    rightMotor->setVelocity(0.0);

    while (rob->step(TIME_STEP) != -1)
    {
        // read dist sensor value
        double psValues[8];
        for (int i = 0; i < 8; i++)
            psValues[i] = ps[i]->getValue();

        // check obstacle using the dist sensor value
        bool right_obstacle = 
            psValues[0] > 80.0 ||
            psValues[1] > 80.0 ||
            psValues[2] > 80.0;
        
        bool left_obstacle = 
            psValues[5] > 80.0 ||
            psValues[6] > 80.0 ||
            psValues[7] > 80.0;

        double leftSpeed = 0.5 * MAX_SPEED;
        double rightSpeed = 0.5 * MAX_SPEED;

        if (left_obstacle)
        {
            leftSpeed = 0.5 * MAX_SPEED;
            rightSpeed = -0.5 * MAX_SPEED;
        }

        if (right_obstacle)
        {
            leftSpeed = -0.5 * MAX_SPEED;
            rightSpeed = 0.5 * MAX_SPEED;
        }

        leftMotor->setVelocity(leftSpeed);
        rightMotor->setVelocity(rightSpeed);
    }
    
    delete rob;
    return 0;
}
