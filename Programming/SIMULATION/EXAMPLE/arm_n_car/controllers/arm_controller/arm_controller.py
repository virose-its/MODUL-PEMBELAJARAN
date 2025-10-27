from controller import Robot
import math


# Create the Robot instance
robot = Robot()

# Get the time step of the current world
timestep = int(robot.getBasicTimeStep())

# Devices
upper_base_imu = robot.getDevice('upper_base_imu')
arm_3_imu = robot.getDevice('arm_3_imu')
upper_base_imu.enable(timestep)
arm_3_imu.enable(timestep)

# Keyboard for input
keyboard = robot.getKeyboard()
keyboard.enable(timestep)

# Motor and sensor names (expected in the robot PROTO)
motor_names = ['upper_base_motor', 'arm_1_motor', 'arm_2_motor', 'arm_3_motor', 'left_gripper_motor', 'right_gripper_motor']
sensor_names = ['upper_base_sensor', 'arm_1_sensor', 'arm_2_sensor', 'arm_3_sensor', 'left_gripper_sensor', 'right_gripper_sensor']

# Map motors and sensors by name for easier access
motors = {}
sensors = {}
for name in motor_names:
    m = robot.getDevice(name)
    if m:
        motors[name] = m
    else:
        print(f"Warning: Motor {name} not found")

for name in sensor_names:
    s = robot.getDevice(name)
    if s:
        s.enable(timestep)
        sensors[name] = s
    else:
        print(f"Warning: Sensor {name} not found")

# Basic parameters (tune to match your robot model)
L1 = 0.15  # length of arm_1 (m)
L2 = 0.15  # length of arm_2 (m)
L3 = 0.08  # length of arm_3 (end effector) (m)

# Joint soft limits (radians) - adjust to match your robot
limits = {
    'arm_1_motor': (-1.57, 1.57),
    'arm_2_motor': (-2.5, 2.5),
    'arm_3_motor': (-1.57, 1.57),
    'upper_base_motor': (-3.14, 3.14)
}

# Movement settings
eef_step = 0.01  # meters per key press step for endpoint
eef_z_step = 0.01
base_rot_speed = 1.0  # rad/s when pressing A/D
grip_speed = 0.5
arm3_speed = 1.0  # rad/s for rotating arm_3 via X/C
gripper_move_speed = 0.5  # rad/s used to increment gripper positions per second

# Accumulated wrist offset (manual adjustment added to IK result)
arm3_offset = 0.0
# HUD / debug settings
hud_enabled = False
hud_counter = 0
HUD_PRINT_INTERVAL = 50  # print status every N simulation steps when HUD enabled

# Gripper joint hard limits (from PROTO) - left and right claws
gripper_limits = {
    'left_gripper_motor': (-1.0, 0.215),
    'right_gripper_motor': (-0.215, 1.0)
}

# Helper: read current joint angles (fallback to 0)
def get_joint_angle(name):
    sname = name.replace('motor', 'sensor')
    s = sensors.get(sname)
    if s:
        return s.getValue()
    # if no sensor, try to approximate with 0
    return 0.0

# Forward kinematics (planar, relative to base rotation)
def forward_kinematics(theta1, theta2, theta3):
    # theta1: shoulder, theta2: elbow, theta3: wrist (all in plane)
    x1 = L1 * math.cos(theta1)
    z1 = L1 * math.sin(theta1)
    x2 = x1 + L2 * math.cos(theta1 + theta2)
    z2 = z1 + L2 * math.sin(theta1 + theta2)
    x3 = x2 + L3 * math.cos(theta1 + theta2 + theta3)
    z3 = z2 + L3 * math.sin(theta1 + theta2 + theta3)
    return x3, z3

# Inverse kinematics for 3-link planar arm. phi is desired end-effector angle
def inverse_kinematics(x, z, phi):
    # compute wrist (arm_3 anchor) position
    x_w = x - L3 * math.cos(phi)
    z_w = z - L3 * math.sin(phi)
    # distance
    r2 = x_w * x_w + z_w * z_w
    # law of cosines for theta2
    denom = 2 * L1 * L2
    if denom == 0:
        return None
    D = (r2 - L1 * L1 - L2 * L2) / denom
    # clamp D
    D = max(-1.0, min(1.0, D))
    theta2 = math.atan2(math.sqrt(max(0.0, 1 - D * D)), D)
    theta1 = math.atan2(z_w, x_w) - math.atan2(L2 * math.sin(theta2), L1 + L2 * math.cos(theta2))
    theta3 = phi - theta1 - theta2
    return theta1, theta2, theta3

# Clamp to limits
def clamp_angle(name, angle):
    if name in limits:
        lo, hi = limits[name]
        return max(lo, min(hi, angle))
    return angle

# Default starting pose for the robot (used as initial IK parameters)
DEFAULT_POSE = {
    'upper_base_motor': 0.0,
    'arm_1_motor': 0.0,
    'arm_2_motor': 0.0,
    'arm_3_motor': 0.0,
    'left_gripper_motor': 0.0,
    'right_gripper_motor': 0.0
}

# Use default pose as starting IK params (overrides sensors at startup)
theta1 = DEFAULT_POSE.get('arm_1_motor', 0.0)
theta2 = DEFAULT_POSE.get('arm_2_motor', 0.0)
theta3 = DEFAULT_POSE.get('arm_3_motor', 0.0)
base_angle = DEFAULT_POSE.get('upper_base_motor', 0.0)

# initial end-effector position in base frame (before base rotation)
eef_x, eef_z = forward_kinematics(theta1, theta2, theta3)
# keep desired end-effector orientation (phi) as the default wrist orientation
phi = theta1 + theta2 + theta3

print("Robot initialized. Starting control loop...")

# Helper to set motor to hold position (use current sensor value)
def hold_motor(name):
    m = motors.get(name)
    if not m:
        return
    angle = get_joint_angle(name)
    m.setPosition(angle)
    m.setVelocity(0.0)

# Main control loop
last_key = None
while robot.step(timestep) != -1:
    dt = timestep / 1000.0

    # read keys
    key = keyboard.getKey()
    # convert printable keycodes to a lowercase char for easier checks
    key_char = None
    if key != -1:
        try:
            kc = chr(key)
            if len(kc) == 1 and kc.isprintable():
                key_char = kc.lower()
        except Exception:
            key_char = None
    # small debug: print when key changes (helps diagnose missing key events)
    if key != -1 and key != last_key:
        if key_char:
            print(f"Key pressed: {key} ('{key_char}')")
        else:
            print(f"Key pressed: {key}")
    # continuous base rotation A/D
    if key == ord('A') or key == ord('a'):
        m = motors.get('upper_base_motor')
        if m:
            m.setPosition(float('inf'))
            m.setVelocity(base_rot_speed)
    elif key == ord('D') or key == ord('d'):
        m = motors.get('upper_base_motor')
        if m:
            m.setPosition(float('inf'))
            m.setVelocity(-base_rot_speed)
    else:
        # stop base rotation and hold
        if 'upper_base_motor' in motors:
            hold_motor('upper_base_motor')

    # rotate arm_3 around its Y axis (manual override)
    # Keys: C (or X) -> positive rotation, V -> negative rotation
    manual_arm3 = False
    if 'arm_3_motor' in motors:
        # adjust the accumulated arm3_offset instead of setting absolute position
        if key_char in ('c', 'x'):
            prev = arm3_offset
            arm3_offset += arm3_speed * dt
            manual_arm3 = True
            print(f"arm_3 offset + : {prev:.3f} -> {arm3_offset:.3f}")
        elif key_char == 'v':
            prev = arm3_offset
            arm3_offset -= arm3_speed * dt
            manual_arm3 = True
            print(f"arm_3 offset - : {prev:.3f} -> {arm3_offset:.3f}")
        elif key_char == 'z':
            # reset offset
            arm3_offset = 0.0
            manual_arm3 = True
            print("arm_3 offset reset to 0.0")
        elif key_char == 'h':
            hud_enabled = not hud_enabled
            print(f"HUD {'enabled' if hud_enabled else 'disabled'}")
        else:
            # fallback to integer key comparison for older behaviour
            if key in (ord('C'), ord('c'), ord('X'), ord('x')):
                prev = arm3_offset
                arm3_offset += arm3_speed * dt
                manual_arm3 = True
                print(f"arm_3 offset + : {prev:.3f} -> {arm3_offset:.3f}")
            elif key in (ord('V'), ord('v')):
                prev = arm3_offset
                arm3_offset -= arm3_speed * dt
                manual_arm3 = True
                print(f"arm_3 offset - : {prev:.3f} -> {arm3_offset:.3f}")

    # gripper open/close E/Q - incremental position control with clamping
    lm = motors.get('left_gripper_motor')
    rm = motors.get('right_gripper_motor')
    if lm and rm and (key in (ord('E'), ord('e')) or key in (ord('Q'), ord('q'))):
        cur_l = get_joint_angle('left_gripper_motor')
        cur_r = get_joint_angle('right_gripper_motor')
        delta = gripper_move_speed * dt
        if key in (ord('E'), ord('e')):
            # open (left moves negative, right moves positive)
            new_l = max(gripper_limits['left_gripper_motor'][0], min(gripper_limits['left_gripper_motor'][1], cur_l - delta))
            new_r = max(gripper_limits['right_gripper_motor'][0], min(gripper_limits['right_gripper_motor'][1], cur_r + delta))
        else:
            # close (left moves positive, right moves negative)
            new_l = max(gripper_limits['left_gripper_motor'][0], min(gripper_limits['left_gripper_motor'][1], cur_l + delta))
            new_r = max(gripper_limits['right_gripper_motor'][0], min(gripper_limits['right_gripper_motor'][1], cur_r - delta))
        lm.setPosition(new_l)
        lm.setVelocity(grip_speed)
        rm.setPosition(new_r)
        rm.setVelocity(grip_speed)
    else:
        # hold current claw positions
        if lm:
            lm_pos = get_joint_angle('left_gripper_motor')
            lm.setPosition(lm_pos)
            lm.setVelocity(0.0)
        if rm:
            rm_pos = get_joint_angle('right_gripper_motor')
            rm.setPosition(rm_pos)
            rm.setVelocity(0.0)

    # Move end-effector target along axes:
    # - W / S: change X axis (forward/back)
    # - R / F: change Z axis (up/down)
    changed = False
    if key_char in ('w',):
        eef_x += eef_step
        changed = True
    elif key_char in ('s',):
        eef_x -= eef_step
        changed = True
    if key_char in ('r',):
        eef_z += eef_z_step
        changed = True
    elif key_char in ('f',):
        eef_z -= eef_z_step
        changed = True

    # fallback to integer codes if key_char is None
    if key_char is None:
        if key in (ord('W'), ord('w')):
            eef_x += eef_step
            changed = True
        elif key in (ord('S'), ord('s')):
            eef_x -= eef_step
            changed = True
        if key in (ord('R'), ord('r')):
            eef_z += eef_z_step
            changed = True
        elif key in (ord('F'), ord('f')):
            eef_z -= eef_z_step
            changed = True

    # Solve IK for the planar arm (base rotation handled separately)
    # The target (eef_x, eef_z) is in the robot base frame (after base rotation)
    sol = inverse_kinematics(eef_x, eef_z, phi)
    if sol is None:
        # nothing to do
        continue
    th1, th2, th3 = sol
    # Clamp angles
    th1 = clamp_angle('arm_1_motor', th1)
    th2 = clamp_angle('arm_2_motor', th2)
    # apply accumulated offset to the IK wrist angle and clamp
    th3_offset = th3 + arm3_offset
    th3_clamped = clamp_angle('arm_3_motor', th3_offset)

    # Apply joint angles to motors (arm_1, arm_2, arm_3)
    if 'arm_1_motor' in motors:
        motors['arm_1_motor'].setPosition(th1)
    if 'arm_2_motor' in motors:
        motors['arm_2_motor'].setPosition(th2)
    if 'arm_3_motor' in motors:
        motors['arm_3_motor'].setPosition(th3_clamped)
        # if clamping changed the desired value, adjust offset to reflect clamp
        if abs(th3_clamped - th3_offset) > 1e-6:
            # reduce offset so final command respects limits
            arm3_offset += (th3_clamped - th3_offset)

    # ensure base holds position if not rotating
    if not (key == ord('A') or key == ord('a') or key == ord('D') or key == ord('d')):
        if 'upper_base_motor' in motors:
            hold_motor('upper_base_motor')

    # small safety: clamp endpoint to reachable workspace
    max_reach = L1 + L2 + L3 - 0.005
    dist = math.hypot(eef_x, eef_z)
    if dist > max_reach:
        scale = max_reach / dist
        eef_x *= scale
        eef_z *= scale

    # optional: print status occasionally
    # print(f"EEF target: x={eef_x:.3f}, z={eef_z:.3f}, theta: {th1:.2f},{th2:.2f},{th3:.2f}")
    # remember last key so we only print key-change messages once
    last_key = key
    # HUD periodic print
    hud_counter += 1
    if hud_enabled and (hud_counter % HUD_PRINT_INTERVAL == 0):
        cur1 = get_joint_angle('arm_1_motor')
        cur2 = get_joint_angle('arm_2_motor')
        cur3 = get_joint_angle('arm_3_motor')
        print(f"HUD: eef(x,z)=({eef_x:.3f},{eef_z:.3f}) phi={phi:.3f} joints=({cur1:.3f},{cur2:.3f},{cur3:.3f}) offset={arm3_offset:.3f}")

