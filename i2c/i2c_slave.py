import smbus
import time
import requests

REQUEST_URL = "https://utev.org/Data?q=setBase&temp="

bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

currentTime = time.time()
endTime = currentTime + 60000

def writeNumber(value):
	bus.write_byte(address, value)
	return -1


def readNumber():
	number = bus.read_byte(address)
	return number

while True:
	if (endTime < currentTime):
		var = 7
		writeNumber(var)
		print("RPI: Hi Arduino, I sent you " + str(var))
		time.sleep(1)
		number = int(readNumber())/10
		print("Arduino: Hey RPI, I received a digit " + str(number))
		print()
		requests.get(REQUEST_URL + str(number))
	else:
		currentTime = time.time()
		print("time difference = " + str(endTime - currentTime))
