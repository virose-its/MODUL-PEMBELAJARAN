from controller import Robot
 
robot = Robot()
timestep = int(robot.getBasicTimeStep())

left_motor = robot.getDevice("left wheel motor")
right_motor = robot.getDevice("right wheel motor")

left_motor.setPosition(float('inf'))
right_motor.setPosition(float('inf'))
left_motor.setVelocity(0.0)
right_motor.setVelocity(0.0)

ds_front_right = robot.getDevice("ds_front_right")
ds_front_left = robot.getDevice("ds_front_left")
ds_left = robot.getDevice("ds_left")
ds_right = robot.getDevice("ds_right")

ds_front_right.enable(timestep)
ds_front_left.enable(timestep)
ds_left.enable(timestep)
ds_right.enable(timestep)

SPEED = 6.0
TURN_SPEED = 4.0
THRESHOLD = 600.0   

while robot.step(timestep) != -1:
    frv = ds_front_right.getValue()
    flv = ds_front_left.getValue()
    lv = ds_left.getValue()
    rv = ds_right.getValue()

    front_val = (flv + frv) / 2.0

    print(f"FR: {frv:.2f}, FL: {flv:.2f}, L: {lv:.2f}, R: {rv:.2f}, F: {front_val:.2f}")

    if frv<THRESHOLD and flv<THRESHOLD:
        if lv>800:            
            left_motor.setVelocity(SPEED-1)
            right_motor.setVelocity(TURN_SPEED-2)
        elif rv>800:
            left_motor.setVelocity(TURN_SPEED-2)
            right_motor.setVelocity(SPEED-1)
        else:
            left_motor.setVelocity(SPEED)
            right_motor.setVelocity(SPEED)
    else:
        if frv > flv and flv < THRESHOLD:
            left_motor.setVelocity(-TURN_SPEED)
            right_motor.setVelocity(TURN_SPEED)
        elif flv > frv and frv < THRESHOLD:
            left_motor.setVelocity(TURN_SPEED)
            right_motor.setVelocity(-TURN_SPEED)
        else:
            if lv < rv:
                left_motor.setVelocity(-TURN_SPEED)
                right_motor.setVelocity(TURN_SPEED)
            else:
                left_motor.setVelocity(TURN_SPEED)
                right_motor.setVelocity(-TURN_SPEED)
    