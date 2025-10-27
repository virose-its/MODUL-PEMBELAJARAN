#include <iostream>
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>

#define TIME_STEP 64
#define MAX_SPEED 6.28

namespace wb = webots;

int main(int argc, char** argv)
{
    wb::Robot* robot = new wb::Robot();

    wb::Motor* leftMotor = robot->getMotor("left wheel motor");
    wb::Motor* rightMotor = robot->getMotor("right wheel motor");

    leftMotor->setPosition(10.0);
    rightMotor->setPosition(10.0);

    // leftMotor->setVelocity(0.1 * MAX_SPEED);
    // rightMotor->setVelocity(0.1 * MAX_SPEED);

    while (robot->step(TIME_STEP) != -1);

    delete robot;

    return 0;
}