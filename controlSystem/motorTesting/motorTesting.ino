#include <Servo.h>

Servo motor1;

int PWMVal = 0;
int percent = 0;

#define motorPin 9

void setup() {

  Serial.begin(9600);
  Serial.println("Welcome to the Motor Control Tester\n");
  //motor1.attach(motorPin);
  Serial.println("To run the program, input a percentage of speed, and the motor will begin spinning.\n");

  motor1.writeMicroseconds(1500);
  motor1.attach(9);

  //analogWrite(motorPin, 0);
}

void loop() {

  int32_t PWMVal = percent * 5 + 1500;

  //analogWrite(motorPin, percent);
  motor1.writeMicroseconds(PWMVal);

  if(Serial.available() > 1) {
    long inputValue = Serial.parseInt();
    if (inputValue == 0){
      Serial.println("Motor Stopping...");
      percent = 0;
    } else if (inputValue >= -100 && inputValue <= 100) {
      percent = inputValue;
      Serial.print("Value set to ");
      Serial.print(percent);
      Serial.println("%");
    } else {
      Serial.println("Invalid Input");
    }

    Serial.print("PWM Val: ");
    Serial.println(PWMVal);
  }
  

}
