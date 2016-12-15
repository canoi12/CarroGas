#include <Ultrasonic.h>

const int motor1[2] = {5, 6};
const int motor2[2] = {7, 8}; 
const int enable = 11;

#define pino_trig 9
#define pino_echo 10
const int pin_a0 = A0;

long duration, distance;
long gas_level, old_gas_level;

void (*actualState)();
void (*oldState)();

void moveFront() {
  digitalWrite(motor1[0], HIGH);
  digitalWrite(motor1[1], LOW);

  digitalWrite(motor2[0], HIGH);
  digitalWrite(motor2[1], LOW);
}

void moveBack() {
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor1[1], HIGH);

  digitalWrite(motor2[0], LOW);
  digitalWrite(motor2[1], HIGH);
}

void moveLeft() {
  digitalWrite(motor1[0], HIGH);
  digitalWrite(motor1[1], LOW);

  digitalWrite(motor2[0], LOW);
  digitalWrite(motor2[1], HIGH);
}

void stopCar() {
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor1[1], LOW);

  digitalWrite(motor2[0], LOW);
  digitalWrite(motor2[1], LOW);
}

void setup() {
  // put your setup code here, to run once:
  gas_level = 0;
  actualState = &moveFront;
  Serial.begin(9600);
  pinMode(motor1[0], OUTPUT);
  pinMode(motor1[1], OUTPUT);

  pinMode(motor2[0], OUTPUT);
  pinMode(motor2[1], OUTPUT);

  pinMode(pino_trig, OUTPUT);
  pinMode(pino_echo, INPUT);

  pinMode(pin_a0, INPUT);

  pinMode(enable,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(enable, HIGH);
  digitalWrite(pino_trig, LOW);
  delayMicroseconds(2);

  digitalWrite(pino_trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(pino_trig, LOW);
  duration = pulseIn(pino_echo, HIGH);
  distance = duration/58.2;

  old_gas_level = gas_level;
  gas_level = analogRead(pin_a0);
  oldState = actualState;

  if (old_gas_level > gas_level + 5) {
    if (actualState == &moveFront && oldState == moveFront) {
      actualState = &moveBack;
    } else if (actualState == moveFront && oldState == moveBack) {
      actualState = &moveLeft;
      actualState();
      delay(2000);
      actualState == &stopCar;
    } else if(actualState == moveBack && oldState == moveBack) {
      actualState = &moveFront;
    } else if (actualState == moveBack && oldState == moveFront) {
      actualState = &moveLeft;
      actualState();
      delay(2000);
      actualState == &stopCar;
    } else if (actualState == stopCar) {
      actualState = &moveFront;
    }
    actualState();
  } else {
    if (actualState == stopCar) {
      actualState = &moveFront;
    }
    actualState();
  }
  if (distance < 5.0) {
    moveLeft();
    delay(2000);
    actualState = &stopCar;
    actualState();
  }

  Serial.print("Gas: ");
  Serial.print( gas_level);
  Serial.print(" Old Gas: ");
  Serial.println(old_gas_level);

  delay(2000);

}
