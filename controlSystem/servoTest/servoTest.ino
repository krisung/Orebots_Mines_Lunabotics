// Include the servo library:
#include <Servo.h>

// Create a new servo object:
Servo myservo;

// Define the servo pin:
#define servoPin 9

// Create a variable to store the servo position:
int angle = 0;
String dir = "";
String forw = "";
String back = "";

void setup() {
  // Attach the Servo variable to a pin:
  myservo.attach(servoPin);
  Serial.begin(9600); 
  
  // Calibrate clockwise command
  Serial.println("Enter forward: ");  
  while(Serial.available() == 0){}
  forw = Serial.readString();
  Serial.println("Entered " + forw);

  // Calibrate counter clockwise command
  Serial.println("Enter backward: ");
  while(Serial.available() == 0){}
  back = Serial.readString();
  Serial.println("Entered " + back);
  myservo.write(0);
}

void loop() {
  while(Serial.available() == 0){}
  dir = Serial.readString();
  Serial.println("Entered " + dir);
  if(dir == forw){
    angle = angle + 20;   // Moves cw by 20 degrees
    myservo.write(angle);
  } else if (dir == back){
    angle = angle - 20;   // Moves ccw by 20 degrees
    myservo.write(angle);
  }
}
