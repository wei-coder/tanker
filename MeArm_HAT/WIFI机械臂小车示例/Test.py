from Adafruit_MotorHAT import Adafruit_MotorHAT, Adafruit_DCMotor,Adafruit_Servo



import time
import atexit
 
# create a default object, no changes to I2C address or frequency
mh = Adafruit_MotorHAT(addr=0x6F,freq=60)

servo0 = mh.getServo(14)
#servo1 = mh.getServo(1)
#servo14 = mh.getServo(14)
#servo15 = mh.getServo(15)

servo0.write(0.8)
time.sleep(3)
servo0.write(1.5)
time.sleep(3)
#servo14.write(2.0)

