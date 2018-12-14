
#!/usr/bin/python
from Adafruit_MotorHAT import Adafruit_MotorHAT, Adafruit_DCMotor
from Adafruit_PWM_Servo_Driver import PWM

import time,sys
import atexit

# create a default object, no changes to I2C address or frequency
mh = Adafruit_MotorHAT(addr=0x6F)

#pwm = PWM(0x6F)

M1 =  mh.getMotor(1)
M2 =  mh.getMotor(2)
M3 =  mh.getMotor(3)
M4 =  mh.getMotor(4)

def forward():
    for i in range(1,5):
        mh.getMotor(i).run(Adafruit_MotorHAT.FORWARD)
	
def backward():
    for i in range(1,5):
        mh.getMotor(i).run(Adafruit_MotorHAT.BACKWARD)

# recommended for auto-disabling motors on shutdown!
def turnOffMotors():
    for i in range(1,5):
         mh.getMotor(i).run(Adafruit_MotorHAT.RELEASE)

atexit.register(turnOffMotors)

def setSpeed(spd):
    for i in range(1,5):
         mh.getMotor(i).setSpeed(spd)

################################# DC motor test!

running = True
speed = 100

M2.run(Adafruit_MotorHAT.FORWARD)
M2.setSpeed(200);
time.sleep(5)
sys.exit()




pwm.setPWMFreq(1000) 

while True:
    try:
        c = raw_input("Enter direction: ")
        if c == '8' :
            print "Running Forward"
            forward()
            setSpeed(speed)
        elif c == '2' :
            print "Running Reverse"
            backward()
            setSpeed(speed)
        elif c == '6' :
            print "Turning Right"
            setSpeed(speed)
            M3.setSpeed(0)
            M4.setSpeed(0)
        elif c == '4' :
            print "Turning Left"
            setSpeed(speed)
            M1.setSpeed(0)
            M2.setSpeed(0)
        elif c == '5' :
           print "Stopped"
           turnOffMotors()
        elif c == '0' :
            print "ServoTest"
            pwm.setPWM(15, 1024, 3072)
    except KeyboardInterrupt:			#Triggered by pressing Ctrl+C
        running = False				#Stop theread1
        print "Bye"
        break					#Exit

