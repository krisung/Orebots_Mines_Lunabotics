#include <ESP32Servo.h>
#define leftMotorPin 25
#define rightMotorPin 12
#define augerDeployPin 26
#define augerSpinnyGuyPin 14 
#define upperStepperPulsePin 18
#define upperStepperDirPin 5
#define lowerStepperPulsePin 16
#define lowerStepperDirPin 4

Servo leftMotors;
Servo rightMotors;
Servo augerSpin;
Servo augerDeploy;

//void CheckForConnections();
//void executeCommand(String command);
String ReceiveData();
void motorStates();
void augerStates();

uint32_t leftPWMDesired = 0;
uint32_t rightPWMDesired = 0;
uint32_t stepperPulseSig = 0;
uint8_t  stepperDir = 0;

int dutyCycle = 128;
const int PWMChannel_Upper = 4;
const int PWMChannel_Lower = 5;
const int PWMResolution = 13;
int PWMFreq_Upper = 0;
int PWMFreq_Lower = 0;

bool augerRotation = false;

enum motorStates_t {WAIT_M, FORWARD, BACKWARD, ROT_RIGHT, ROT_LEFT};
enum augerStates_t {WAIT_A, DEPLOY, UNDEPLOY, BEG_ROT, STOP_ROT, LOWER_BOT, RAISE_BOT, LOWER_TOP, RAISE_TOP};
//enum stepperTestStates_t {WAIT_S, FOR, BAC};
motorStates_t motorState = WAIT_M;
//stepperTestStates_t stepperState = WAIT_S;
augerStates_t augerState = WAIT_A;

void setup() {
  leftMotors.writeMicroseconds(1500);
  rightMotors.writeMicroseconds(1500);
  augerSpin.writeMicroseconds(1500);
  augerDeploy.writeMicroseconds(1500);
  



  ledcSetup(PWMChannel_Upper, PWMFreq_Upper, PWMResolution);
  ledcSetup(PWMChannel_Lower, PWMFreq_Lower, PWMResolution);

  ledcAttachPin(upperStepperPulsePin, PWMChannel_Upper);
  ledcAttachPin(lowerStepperPulsePin, PWMChannel_Lower);

  leftMotors.attach(leftMotorPin);
  rightMotors.attach(rightMotorPin);
  augerSpin.attach(augerSpinnyGuyPin);
  augerDeploy.attach(augerDeployPin);
}

void loop() {
  motorState = WAIT_M;
  augerState = WAIT_A;
  motorStates();
  augerStates();
  delay(1000);
  motorState = FORWARD;
  augerState = DEPLOY;
  motorStates();
  augerStates();
  delay(1000);
  motorState = BACKWARD;
  augerState = UNDEPLOY;
  motorStates();
  augerStates();
  delay(1000);
  motorState = ROT_RIGHT;
  augerState = LOWER_BOT;
  motorStates();
  augerStates();
  delay(1000);
  motorState = ROT_LEFT;
  augerState = RAISE_BOT;
  motorStates();
  augerStates();
  delay(1000);
  motorState = FORWARD;
  augerState = LOWER_TOP;
  motorStates();
  augerStates();
  delay(1000);
  motorState = BACKWARD;
  augerState = RAISE_TOP;
  motorStates();
  augerStates();
  delay(1000);
  motorState = ROT_RIGHT;
  augerState = WAIT_A;
  augerRotation = true;
  motorStates();
  augerStates();
  delay(2000);
  motorState = ROT_LEFT;
  augerState = WAIT_A;
  augerRotation = false;
  motorStates();
  augerStates();
  delay(1000);
}

void motorStates(){
  switch(motorState){
    case WAIT_M:
      leftPWMDesired = 1500;
      rightPWMDesired = 1500;
     break;
    case FORWARD:
      leftPWMDesired = 1600;
      rightPWMDesired = 1400;
      break;
    case BACKWARD:
      leftPWMDesired = 1400;
      rightPWMDesired = 1600;
      break;
    case ROT_LEFT:
      leftPWMDesired = 1400;
      rightPWMDesired = 1400;
      break;
    case ROT_RIGHT:
      leftPWMDesired = 1600;
      rightPWMDesired = 1600;
      break;
  }

  leftMotors.writeMicroseconds(leftPWMDesired);
  rightMotors.writeMicroseconds(rightPWMDesired); 

  //Serial.print("Writing left: ");
  //Serial.print(leftPWMDesired);
  //Serial.print(" Right: ");
  //Serial.println(rightPWMDesired);

}


void augerStates(){
  switch(augerState){
    case WAIT_A:
      ledcChangeFrequency(PWMChannel_Upper, 0, PWMResolution);
      ledcChangeFrequency(PWMChannel_Lower, 0, PWMResolution);
      ledcWrite(PWMChannel_Upper, 4096);
      ledcWrite(PWMChannel_Lower, 4096);
      augerDeploy.writeMicroseconds(1500);
      break;
    case DEPLOY:
      augerDeploy.writeMicroseconds(1400);
//      Serial.println("Wrote Deploy");
      break;
    case UNDEPLOY:
      augerDeploy.writeMicroseconds(1600);
//      Serial.println("Wrote Undeploy");
      break;
    case LOWER_BOT:
      ledcChangeFrequency(PWMChannel_Lower, 1500, PWMResolution);
      ledcWrite(PWMChannel_Lower, 4096);
      analogWrite(lowerStepperDirPin, 0);
//      Serial.println("Wrote Lowering");
      break;
    case RAISE_BOT:
      ledcChangeFrequency(PWMChannel_Lower, 1500, PWMResolution);
      ledcWrite(PWMChannel_Lower, 4096);
      analogWrite(lowerStepperDirPin, 256);
//      Serial.println("Wrote Raising");
      break;
    case LOWER_TOP:
      ledcChangeFrequency(PWMChannel_Upper, 1500, PWMResolution);
      ledcWrite(PWMChannel_Upper, 4096);
      analogWrite(upperStepperDirPin, 0);
//      Serial.println("Wrote Lowering");
      break;
    case RAISE_TOP:
      ledcChangeFrequency(PWMChannel_Upper, 1500, PWMResolution);
      ledcWrite(PWMChannel_Upper, 4096);
      analogWrite(upperStepperDirPin, 256);
//      Serial.println("Wrote Raising");
      break;
  }
  if(augerRotation == true){
    augerSpin.writeMicroseconds(1400);
    //Serial.println("Wrote Spin");
  } else {
    augerSpin.writeMicroseconds(1500);
    //Serial.println("Wrote Stop");
  }
}
