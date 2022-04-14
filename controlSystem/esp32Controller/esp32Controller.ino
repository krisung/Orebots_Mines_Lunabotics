#include <SPI.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include "ArduinoTimer.h"
#include "MegunoLink.h"

#define leftMotorPin 25
#define rightMotorPin 33
#define augerDeployPin 26
#define augerSpinnyGuyPin 14 
#define upperStepperPulsePin 18
#define upperStepperDirPin 5 
#define lowerStepperPulsePin 16
#define lowerStepperDirPin 4

const uint ServerPort = 80;
WiFiServer Server(ServerPort);
WiFiClient RemoteClient;
ArduinoTimer SendTimer;

Servo leftMotors;
Servo rightMotors;
Servo augerSpin;
Servo augerDeploy;

void CheckForConnections();
void executeCommand(String command);
String ReceiveData();
void motorStates();
void augerStates();

const char ssid[] = "Lunabotics_Selene_2.4";
const char pass[] = "MinesLunaboticsWifi1!";
//const char ssid[] = "iPhone";
//const char pass[] = "pleasework";
IPAddress ip(192, 168, 0, 103);
IPAddress gateway(192, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);
int status = WL_IDLE_STATUS;

uint32_t leftPWMDesired = 0;
uint32_t rightPWMDesired = 0;
uint32_t stepperPulseSig = 0;
uint8_t  stepperDir = 0;

int dutyCycle = 128;
const int PWMChannel_Upper = 4;
const int PWMChannel_Lower = 5;
const int PWMResolution = 13;
int PWMFreq_Upper = 1500;
int PWMFreq_Lower = 1500;

bool augerRotation = false;

enum motorStates_t {WAIT_M, FORWARD, BACKWARD, ROT_RIGHT, ROT_LEFT};
enum augerStates_t {WAIT_A, DEPLOY, UNDEPLOY, BEG_ROT, STOP_ROT, LOWER_BOT, RAISE_BOT, LOWER_TOP, RAISE_TOP};
//enum stepperTestStates_t {WAIT_S, FOR, BAC};
motorStates_t motorState = WAIT_M;
//stepperTestStates_t stepperState = WAIT_S;
augerStates_t augerState = WAIT_A;

//# Command Prompts
//# 0 - IDLE
//# 1 - Forward
//# 2 - Backward
//# 3 - Left
//# 4 - Right
//# 5 - Rotate Auger for Deployment
//# 6 - Undeploy Auger
//# 7 - Begin/Stop rotating auger
//# 8 - Lower Stage Auger Down
//# 9 - Lower Stage Auger Up
//# A - Upper Stage Auger Down
//# B - Upper Stage Auger Up

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to network");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(F("."));
  }
  
  Server.begin();
  Serial.print("Connected to wifi. My address: ");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);

  leftMotors.writeMicroseconds(1500);
  rightMotors.writeMicroseconds(1500);
  augerSpin.writeMicroseconds(1500);
  augerDeploy.writeMicroseconds(1500);

  leftMotors.attach(leftMotorPin);
  rightMotors.attach(rightMotorPin);
  augerSpin.attach(augerSpinnyGuyPin);
  augerDeploy.attach(augerDeployPin);

  ledcSetup(PWMChannel_Upper, PWMFreq_Upper, PWMResolution);
  ledcSetup(PWMChannel_Lower, PWMFreq_Lower, PWMResolution);

  ledcAttachPin(upperStepperPulsePin, PWMChannel_Upper);
  ledcAttachPin(lowerStepperPulsePin, PWMChannel_Lower);
  

}

void loop() {
  String command;
  
  CheckForConnections();
  
  if(RemoteClient){
    while(RemoteClient.connected()){
      while(RemoteClient.available()!=0){
        command = ReceiveData(); 
      }
    }

    RemoteClient.stop();
    Serial.println("Client disconnected");
  }
}

void CheckForConnections(){
  if (Server.hasClient()) {
    if (RemoteClient.connected()){  // Prevents connection from multiple computers
      Serial.println("Connection rejected");
      Server.available().stop();
    } else {
      Serial.println("Connection accepted");
      RemoteClient = Server.available();
    }
  }
}

void executeCommand(String command){
  char trimmedCommand = command[0];
  //Serial.println(trimmedCommand);
  if(trimmedCommand == '0'){
    //Serial.println("Stopped");
    motorState = WAIT_M;
    augerState = WAIT_A;
  } else if(trimmedCommand == '1') {
    //Serial.println("Moving Forward");
    motorState = FORWARD;
  } else if(trimmedCommand == '2'){
    //Serial.println("Moving Backward");
    motorState = BACKWARD;
  } else if(trimmedCommand == '3'){
    //Serial.println("Rotating Left");
    motorState = ROT_LEFT;
  } else if(trimmedCommand == '4') {
    //Serial.println("Rotating Right");
    motorState = ROT_RIGHT;
  }else if(trimmedCommand == '5'){
//    Serial.println("Deploying");
    augerState = DEPLOY;
  }else if(trimmedCommand == '6'){
//    Serial.println("UnDeploying");
    augerState = UNDEPLOY;
  }else if(trimmedCommand == '7'){
    if (augerRotation == true){
//      Serial.println("Stop Rotation");
      augerRotation == false;
    } else {
//      Serial.println("Start Rotation");
      augerRotation == true;
    }
  } else if(trimmedCommand == '8'){
    //Serial.println("Lower Bottom Stage");
    augerState = LOWER_BOT;
  } else if(trimmedCommand == '9'){
    //Serial.println("Raising Bottom Stage");
    augerState = RAISE_BOT;
  } else if(trimmedCommand == 'A'){
    //Serial.println("Lower Top Stage");
    augerState = LOWER_TOP;
  } else if(trimmedCommand == 'B'){
    //Serial.println("Raise Top Stage");
    augerState = RAISE_TOP;
  }//else{
   // Serial.print("Invalid Command, received: ");
   // Serial.println(trimmedCommand);
  //}

  motorStates();
  augerStates();
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
  //Serial.println(augerState);
  switch(augerState){
    case WAIT_A:
      //ledcChangeFrequency(PWMChannel_Upper, 0, PWMResolution);
      //ledcChangeFrequency(PWMChannel_Lower, 0, PWMResolution);
      ledcWrite(PWMChannel_Upper, 8192);
      ledcWrite(PWMChannel_Lower, 8192);
      augerDeploy.writeMicroseconds(1500);
      break;
    case DEPLOY:
      augerDeploy.writeMicroseconds(1400);
      //Serial.println("Wrote Deploy");
      break;
    case UNDEPLOY:
      augerDeploy.writeMicroseconds(1600);
      //Serial.println("Wrote Undeploy");
      break;
    case LOWER_BOT:
      //Serial.println("Wrote Lowering");
      //ledcChangeFrequency(PWMChannel_Lower, 1500, PWMResolution);
      ledcWrite(PWMChannel_Lower, 4096);
      analogWrite(lowerStepperDirPin, 0);
      break;
    case RAISE_BOT:
      //Serial.println("Wrote Raising");
      //ledcChangeFrequency(PWMChannel_Lower, 1500, PWMResolution);
      ledcWrite(PWMChannel_Lower, 4096);
      analogWrite(lowerStepperDirPin, 256);
      break;
    case LOWER_TOP:
      //Serial.println("Wrote Lowering");
      //ledcChangeFrequency(PWMChannel_Upper, 1500, PWMResolution);
      ledcWrite(PWMChannel_Upper, 4096);
      analogWrite(upperStepperDirPin, 0);
      break;
    case RAISE_TOP:
      //Serial.println("Wrote Raising");
      //ledcChangeFrequency(PWMChannel_Upper, 1500, PWMResolution);
      ledcWrite(PWMChannel_Upper, 4096);
      analogWrite(upperStepperDirPin, 256);
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

String ReceiveData(){
  char dataBuffer[25];
  int index = 0;
  while(RemoteClient.available()>0){
    dataBuffer[index] = RemoteClient.read();
    index += 1;
  }
  String command = String(dataBuffer);
  executeCommand(command);
  return command;  
}
