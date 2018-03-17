import RPi.GPIO as GPIO
import time

OUTPUT_PIN = 18

turns = 0           # how fast the motor runs
turnAmount = 1      # how many turns the motor makes
currentTime = -1
loopTime = -1

def setup():
    GPIO.setup(OUTPUT_PIN, GPIO.OUT)
    currentTime = time.now()
    loopTime = currentTime

def loop():
    currentTime = time.now()
    if ( currentTime >= (loopTime + 20) ):
        GPIO.OUT(OUTPUT_PIN, GPIO.HIGH)

        turns = turns + turnAmount

        if (turns == 0 || turns == 255):
            turnAmount = -turnAmount

        if (turns == 0):
            time.wait(5000)

        loopTime = currentTime
    else:
        GPIO.OUT(OUTPUT_PIN, GPIO.LOW)
