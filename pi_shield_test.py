#!/usr/bin/env python3
import time
import signal
import sys
from gpiozero import PWMOutputDevice

# =========================
# BCM GPIO pins (as you specified)
# =========================
MOTOR_A_IN1 = 17
MOTOR_A_IN2 = 27
MOTOR_B_IN1 = 13
MOTOR_B_IN2 = 19

# Speed for testing (0.0 to 1.0). Start low to be safe.
TEST_DUTY = 0.30

# Timing (seconds)
RUN_TIME  = 2.0
STOP_TIME = 1.0

a1 = PWMOutputDevice(MOTOR_A_IN1, frequency=2000, initial_value=0)
a2 = PWMOutputDevice(MOTOR_A_IN2, frequency=2000, initial_value=0)
b1 = PWMOutputDevice(MOTOR_B_IN1, frequency=2000, initial_value=0)
b2 = PWMOutputDevice(MOTOR_B_IN2, frequency=2000, initial_value=0)

def all_stop():
    for dev in (a1, a2, b1, b2):
        dev.value = 0

def set_motor(in1: PWMOutputDevice, in2: PWMOutputDevice, speed: float):
    """
    speed: -1.0..+1.0
    + = IN1 PWM, IN2 off
    - = IN2 PWM, IN1 off
    0 = both off (coast)
    """
    speed = max(-1.0, min(1.0, speed))
    if speed > 0:
        in1.value = abs(speed)
        in2.value = 0
    elif speed < 0:
        in1.value = 0
        in2.value = abs(speed)
    else:
        in1.value = 0
        in2.value = 0

def handle_exit(sig, frame):
    all_stop()
    sys.exit(0)

signal.signal(signal.SIGINT, handle_exit)
signal.signal(signal.SIGTERM, handle_exit)

print("Pi Shield Test starting (CTRL+C to stop).")
all_stop()
time.sleep(0.5)

try:
    while True:
        # Motor A forward
        print("Motor A: forward")
        set_motor(a1, a2, +TEST_DUTY)
        time.sleep(RUN_TIME)
        all_stop()
        time.sleep(STOP_TIME)

        # Motor A reverse
        print("Motor A: reverse")
        set_motor(a1, a2, -TEST_DUTY)
        time.sleep(RUN_TIME)
        all_stop()
        time.sleep(STOP_TIME)

        # Motor B forward
        print("Motor B: forward")
        set_motor(b1, b2, +TEST_DUTY)
        time.sleep(RUN_TIME)
        all_stop()
        time.sleep(STOP_TIME)

        # Motor B reverse
        print("Motor B: reverse")
        set_motor(b1, b2, -TEST_DUTY)
        time.sleep(RUN_TIME)
        all_stop()
        time.sleep(STOP_TIME)

except KeyboardInterrupt:
    pass
finally:
    all_stop()
