from controller import Robot, Keyboard
from enum import Enum, auto
import os

# Enable keyboard

class steer_mode_enum(Enum):
    FRONT_STEER = 0
    REAR_STEER = auto()
    FRONT_REAR_STEER = auto()
    CRAB_STEER = auto()
steer_mode_count = len(steer_mode_enum)

robot = Robot()
timestep = int(robot.getBasicTimeStep())

keyboard = Keyboard()
keyboard.enable(timestep)
dict_keys = {
    'W': ord('W'),
    'S': ord('S'),
    'A': ord('A'),
    'D': ord('D'),
    'E': ord('E'),
    'V': ord('V')
}

# Device names
motor_names = ['front_left', 'front_right', 'rear_left', 'rear_right']
# Get devices
wheel_motors = {name+"_wheel": robot.getDevice(f'{name}_wheel') for name in motor_names}
steer_motors = {name+"_steer": robot.getDevice(f'{name}_steer') for name in motor_names}
imu = robot.getDevice('inertial_unit')
imu.enable(timestep)
gyro = robot.getDevice('gyro')
gyro.enable(timestep)
accelerometer = robot.getDevice('accelerometer')
accelerometer.enable(timestep)


# Control parameters
base_speed = 30
steer_angle = 0.3
speed = 0.0
turn = 0.0
steer_mode = steer_mode_enum.FRONT_STEER
current_speed = 0.0
inertia = 0.97

def clear_console():
    os.system('cls')

def take_input():
    key = keyboard.getKey()
    keys = set()
    while key != -1:
        keys.add(key)
        key = keyboard.getKey()
    return keys

def steering_mode(current_mode):
    mode = steer_mode_enum((current_mode.value + 1) % steer_mode_count)
    return mode

def set_steering(steer_mode, turn):
    # Reset all steering
    for motor in steer_motors.values():
        if motor:
            motor.setPosition(0.0)
    # Apply steering based on mode
    if steer_mode == steer_mode_enum.FRONT_STEER:
        if steer_motors['front_left_steer']:
            steer_motors['front_left_steer'].setPosition(turn)
        if steer_motors['front_right_steer']:
            steer_motors['front_right_steer'].setPosition(turn)
    elif steer_mode == steer_mode_enum.REAR_STEER:
        if steer_motors['rear_left_steer']:
            steer_motors['rear_left_steer'].setPosition(-turn)
        if steer_motors['rear_right_steer']:
            steer_motors['rear_right_steer'].setPosition(-turn)
    elif steer_mode == steer_mode_enum.FRONT_REAR_STEER:
        if steer_motors['front_left_steer']:
            steer_motors['front_left_steer'].setPosition(turn)
        if steer_motors['front_right_steer']:
            steer_motors['front_right_steer'].setPosition(turn)
        if steer_motors['rear_left_steer']:
            steer_motors['rear_left_steer'].setPosition(-turn)
        if steer_motors['rear_right_steer']:
            steer_motors['rear_right_steer'].setPosition(-turn)
    elif steer_mode == steer_mode_enum.CRAB_STEER:
        for motor in steer_motors.values():
            if motor:
                motor.setPosition(turn)

def set_speed(speed):
    for motor in wheel_motors.values():
        if motor:
            motor.setPosition(float('inf'))
            motor.setVelocity(speed)

E_pressed = False
while robot.step(timestep) != -1:
    movement = 'neutral'
    steering = 'straight'
    steering_mode_str = steer_mode_enum(steer_mode).name
    boost = 'False'

    target_speed = 0.0
    turn = 0.0

    keys = take_input()

    if dict_keys['W'] in keys:
        target_speed = base_speed
        movement = 'forward'
        
    if dict_keys['S'] in keys:
        target_speed = -base_speed
        movement = 'backward'
    if dict_keys['V'] in keys:
        target_speed = target_speed * 2
        boost = 'True'

    # Steering logic: A = left, D = right, both = neutral
    if dict_keys['A'] in keys and dict_keys['D'] not in keys:
        turn = steer_angle
        steering = 'left'
    elif dict_keys['D'] in keys and dict_keys['A'] not in keys:
        turn = -steer_angle
        steering = 'right'
    else:
        turn = 0.0

    # Cycle steering mode
    if dict_keys['E'] in keys and not E_pressed:
        steer_mode=steering_mode(steer_mode)
        E_pressed = True
    elif dict_keys['E'] not in keys:
        E_pressed = False
    print(1)
    # Inertia: blend current speed toward target speed
    current_speed = inertia * current_speed + (1 - inertia) * target_speed

    set_steering(steer_mode, turn)
    set_speed(current_speed)
    
    # Interface
    imu_values = imu.getRollPitchYaw()
    accel_values = accelerometer.getValues()
    gyro_values = gyro.getValues()

    print(f"{'CONTROLLER':=^20}")
    print('W/S = forward/back, A/D = steer, C = boost, E = cycle steering mode')
    print(f'Steering Mode: {steering_mode_str} \nMovement: {movement} \nSteering: {steering} \nBoost: {boost}')
    print("IMU Roll: {:.2f}, Pitch: {:.2f}, Yaw: {:.2f}".format(imu_values[0], imu_values[1], imu_values[2]))
    print("Accelerometer X: {:.2f}, Y: {:.2f}, Z: {:.2f}".format(accel_values[0], accel_values[1], accel_values[2]))
    print("Gyro X: {:.2f}, Y: {:.2f}, Z: {:.2f}".format(gyro_values[0], gyro_values[1], gyro_values[2]))
