#include <SPI.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include "ArduinoTimer.h"
#include "MegunoLink.h"

#define leftMotorPin 32
#define rightMotorPin 12

const uint ServerPort = 80;
WiFiServer Server(ServerPort);
WiFiClient RemoteClient;
ArduinoTimer SendTimer;

Servo leftMotors;
Servo rightMotors;

void CheckForConnections();
void executeCommand(String command);
String ReceiveData();
void motorStates();

const char ssid[] = "Lunabotics_Selene_2.4";
const char pass[] = "MinesLunaboticsWifi1!";
IPAddress ip(192, 168, 0, 103);
IPAddress gateway(192, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);
int status = WL_IDLE_STATUS;

uint32_t leftPWMDesired = 0;
uint32_t rightPWMDesired = 0;

enum motorStates_t {WAIT, FORWARD, BACKWARD, ROT_RIGHT, ROT_LEFT};
motorStates_t motorState = WAIT;

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
  leftMotors.attach(leftMotorPin);
  rightMotors.attach(rightMotorPin);
}

void loop() {
  String command;
  
  CheckForConnections();
  
  if(RemoteClient){
    while(RemoteClient.connected()){
      while(RemoteClient.available()!=0){
        command = ReceiveData(); 
      }
      motorStates();
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
  if(trimmedCommand == '0'){
    //Serial.println("Stopped");
    motorState = WAIT;
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
  }else {
    //Serial.print("Invalid Command, received: ");
    //Serial.println(trimmedCommand);
  }
}

void motorStates(){
  switch(motorState){
    case WAIT:
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
