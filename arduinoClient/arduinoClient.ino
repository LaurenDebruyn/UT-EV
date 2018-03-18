#include <math.h>
#include <SimpleTimer.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x04

const unsigned long interval = 10;
const unsigned long nbIntervalsSuntiles = 5;
const unsigned long durationIntervalSuntiles = 1200;
const unsigned long noMovingTilesInterval = 120000;

const int pin8 = 8;
const int pin9 = 9;
const int pin10 = 10;
const int pin11 = 11;
const int B = 4275;
const int R0 = 100000;
const int pinTempSensor = A1;

int number = 0;

SimpleTimer timer;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  // adjust suntiles every 10 minutes
  timer.setInterval(noMovingTilesInterval, positioningSuntiles);
  
  Serial.println("Ready!");
}

void loop() {
  timer.run();
  delay(100);
}

// callback for received data
void receiveData(int byteCount) {
  while(Wire.available()) {
    number = Wire.read();
    Serial.print("data received:");
    Serial.println(number);
  }
}

// callback for sending data
void sendData(){
  if (number == 7) {
    int temp = int(getTemperature()*10);
    Wire.write(temp);
  } else {
    Wire.write(number);
  }
}

float getTemperature() {
  int a = analogRead(pinTempSensor);
  
  float R = 1023.0/a-1.0;
  R = R0*R;
  
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
  
  Serial.print("temperature = ");
  Serial.println(temperature);
  
  return temperature;
}

void turn0() {
  digitalWrite(pin8, HIGH);
  digitalWrite(pin9, LOW);
  digitalWrite(pin10, LOW);
  digitalWrite(pin11, LOW);
}

void turn1() {
  digitalWrite(pin8, LOW);
  digitalWrite(pin9, HIGH);
  digitalWrite(pin10, LOW);
  digitalWrite(pin11, LOW);
}

void turn2() {
  digitalWrite(pin8, LOW);
  digitalWrite(pin9, LOW);
  digitalWrite(pin10, HIGH);
  digitalWrite(pin11, LOW);
}


void turn3() {
  digitalWrite(pin8, LOW);
  digitalWrite(pin9, LOW);
  digitalWrite(pin10, LOW);
  digitalWrite(pin11, HIGH);
}

void rotateLeft() {
  turn0();
  delay(interval);
  turn1();
  delay(interval);
  turn2();
  delay(interval);
  turn3();
  delay(interval);
}

void rotateRight() {
  turn3();
  delay(interval);
  turn2();
  delay(interval);
  turn1();
  delay(interval);
  turn0();
  delay(interval);
}

void positioningSuntiles() {
  Serial.println("position suntiles");
  int currentPosition = 0;
  int bestPosition;
  int maxLightIntensity = 0;
  int currentLightIntensity;
  
  while (currentPosition<nbIntervalsSuntiles) {
    unsigned long startTime = millis();
    while(millis() <= startTime + durationIntervalSuntiles) {
      rotateLeft();
    }
    currentLightIntensity = getLightIntensity();
    if (currentLightIntensity >= maxLightIntensity) {
      bestPosition = currentPosition;
      maxLightIntensity = currentLightIntensity;
    }
    currentPosition++;
  }
  
  int rotationsBack = nbIntervalsSuntiles - bestPosition - 1;
  
  while (rotationsBack>0) {
    rotateRight();
    rotationsBack--;
  }
  Serial.println("rotation suntiles finished");
}

int getLightIntensity() {
  int value = analogRead(A0);                                                                               
  Serial.print("light: ");
  Serial.println(value);
  return value;
}


