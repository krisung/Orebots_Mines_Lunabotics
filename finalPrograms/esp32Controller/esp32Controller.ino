//------------------------------------------------------------
//    ESP32 Controller
//------------------------------------------------------------
//    Written By: Kristen Ung, CSMOrebots
//    Email: kristen.ung23@gmail.com
//
//    The purpose of this program is to act as a controller
//    for the NASA Lunabotics Competition for the Colorado
//    School of Mines. The program opens a port for the 
//    ground station to socket to, and recieves various
//    commands based on keyboard inputs (See GroundStation.py).
//    
//    When a command is received, the string is interpreted and
//    Sets different states based on what was sent. There are 
//    3 main state machines, one for the drive train, one for the
//    mining system, and one for the collection system. Motor
//    signals are set based on which state the machine is in.
//
//    Pin Assignments:
//    Pin 25 -> Left Drive Train
//    Pin 33 -> Right Drive Train
//    Pin 26 -> Auger Deployment Motor
//    Pin 14 -> Auger Drill Rotation Motor
//    Pin 18 -> Upper Stage Stepper Motor Pulses
//    Pin  5 -> Upper Stage Stepper Motor Direction
//    Pin 16 -> Lower Stage Stepper Motor Pulses
//    Pin  4 -> Lower Stage Stepper Motor Direction
//    Pin 12 -> Collection System Linear Actuators
//    Pin 13 -> Collection System Belt Rotation
//
//    # Command Prompts
//    # 0 - IDLE
//    # 1 - Forward
//    # 2 - Backward
//    # 3 - Left
//    # 4 - Right
//    # 5 - Rotate Auger for Deployment
//    # 6 - Undeploy Auger
//    # 7 - Begin/Stop rotating auger
//    # 8 - Lower Stage Auger Down
//    # 9 - Lower Stage Auger Up
//    # A - Upper Stage Auger Down
//    # B - Upper Stage Auger Up
//
//------------------------------------------------------------


#include <SPI.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include "ArduinoTimer.h"
#include "MegunoLink.h"

// Pin Declarations
#define leftMotorPin 25        
#define rightMotorPin 33          
#define augerDeployPin 26        
#define augerSpinnyGuyPin 14 
#define upperStepperPulsePin 18
#define upperStepperDirPin 5 
#define lowerStepperPulsePin 16
#define lowerStepperDirPin 4
#define linearActuatorsPin 12
#define collectionSpinPin 13

// Variable Declarations For Connecting to Ground Station
const uint ServerPort = 80;
WiFiServer Server(ServerPort);
WiFiClient RemoteClient;
ArduinoTimer SendTimer;
IPAddress ip(192, 168, 0, 103);
IPAddress gateway(192, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);
int status = WL_IDLE_STATUS;

// You may need to change these values based on the WiFi you are
// connecting over
const char ssid[] = "Lunabotics_Selene_2.4"; // WiFi SSID
const char pass[] = "MinesLunaboticsWifi1!"; // WiFi Password

// Initializing all signals for Talon SRX controllers
Servo leftMotors;       //  -> Left Drive Train
Servo rightMotors;      //  -> Right Drive Train
Servo augerSpin;        //  -> Auger Drill Rotation
Servo augerDeploy;      //  -> Auger Deployment Motor
Servo linearActuators;  //  -> Collection System Linear Actuators
Servo collectionSpin;   //  -> Collection System Belt Rotation

// Function Declarations
void CheckForConnections();
void executeCommand(String command);
String ReceiveData();
void motorStates();
void augerStates();
void collectionStates();

// Initialize Duty Cycles for PWMs
uint32_t leftPWMDesired = 0;
uint32_t rightPWMDesired = 0;
uint32_t stepperPulseSig = 0;
uint8_t  stepperDir = 0;

// Initialize Values to create frequencies for Stepper motors
int dutyCycle = 128;
const int PWMChannel_Upper = 4;
const int PWMChannel_Lower = 5;
const int PWMResolution = 13;
int PWMFreq_Upper = 2000;
int PWMFreq_Lower = 2000;

bool augerRotation = false;

// Enumerate States to make state machines more readable
enum motorStates_t {WAIT_M, FORWARD, BACKWARD, ROT_RIGHT, ROT_LEFT};
enum augerStates_t {WAIT_A, DEPLOY, UNDEPLOY, BEG_ROT, STOP_ROT, LOWER_BOT, RAISE_BOT, LOWER_TOP, RAISE_TOP};
enum collectionStates_t {WAIT_C, EXTEND_ACT, CONTRACT_ACT, ROTATE_YEET, ROTATE_ADJUST};
motorStates_t motorState = WAIT_M;
augerStates_t augerState = WAIT_A;
collectionStates_t collectionState = WAIT_C;


// The setup function is run at the startup of the program. The purpose is to
// set up the wireless network, as well as connect the motors to the pins of
// the ESP32.
void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to network");
  Serial.print("SSID: ");
  Serial.println(ssid);

  // Attempts to connect to WiFi network
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  // Prints dots until WiFi is connected to show the system
  // is still responding, and waiting for connection
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(F("."));
  }
  
  // WiFi has connected, and the IP address will print to the terminal
  Server.begin();
  Serial.print("Connected to wifi. My address: ");
  IPAddress myAddress = WiFi.localIP();
  Serial.println(myAddress);

  // Initialized Motor values to 0 for setup
  leftMotors.writeMicroseconds(1500);
  rightMotors.writeMicroseconds(1500);
  augerSpin.writeMicroseconds(1500);
  augerDeploy.writeMicroseconds(1500);
  linearActuators.writeMicroseconds(1500);
  collectionSpin.writeMicroseconds(1500);

  // Attaches Motors to Specific Pins
  leftMotors.attach(leftMotorPin);
  rightMotors.attach(rightMotorPin);
  augerSpin.attach(augerSpinnyGuyPin);
  augerDeploy.attach(augerDeployPin);
  linearActuators.attach(linearActuatorsPin);
  collectionSpin.attach(collectionSpinPin);

  // Creates channels for the stepper motor frequencies
  ledcSetup(PWMChannel_Upper, PWMFreq_Upper, PWMResolution);
  ledcSetup(PWMChannel_Lower, PWMFreq_Lower, PWMResolution);

  // Attaches the stepper motor pins to the channels
  ledcAttachPin(upperStepperPulsePin, PWMChannel_Upper);
  ledcAttachPin(lowerStepperPulsePin, PWMChannel_Lower);

}

// This is the main program function which runs on the ESP32
void loop() {
  String command;
  
  // Checks to see if the ground station has connection to the controller
  CheckForConnections();
  
  // If there is a connection, check to see if a command has been received
  if(RemoteClient){
    while(RemoteClient.connected()){
      while(RemoteClient.available()!=0){
        command = ReceiveData();  // Calls function to collect and trim command
      }
    }

    // Disconnect if the ground station disconnects
    RemoteClient.stop();
    Serial.println("Client disconnected");
  }
}

// Function to allow for ground station connection
void CheckForConnections(){
  if (Server.hasClient()) {
    if (RemoteClient.connected()){  
      Serial.println("Connection rejected");
      Server.available().stop();   // Prevents multiple connections to controller
    } else {
      Serial.println("Connection accepted");
      RemoteClient = Server.available();  // Keeps a server available until connected
    }
  }
}

// Function for receiving command
String ReceiveData(){
  char dataBuffer[25];  // Sets max buffer size (should never be reached)
  int index = 0;   
  while(RemoteClient.available()>0){
    dataBuffer[index] = RemoteClient.read();  // Reads data from Client
    index += 1;
  }
  String command = String(dataBuffer);    // Sets command from data buffer
  executeCommand(command);    // Tells system to execute command
  return command;  
}

//Function for executing the received Command
void executeCommand(String command){
  char trimmedCommand = command[0]; // Only looks at first character (the input command)

  if(trimmedCommand == '0'){   

    //----------------------------
    //  IDLE STATE
    //----------------------------

    motorState = WAIT_M;                 
    augerState = WAIT_A;
    collectionState = WAIT_C;
  } else if(trimmedCommand == '1') { 

    //----------------------------
    //  FORWARD MOVEMENT 
    //----------------------------
    
    motorState = FORWARD;                 
  } else if(trimmedCommand == '2'){  

    //----------------------------
    //  BACKWARD MOVEMENT
    //----------------------------
     
    motorState = BACKWARD;                      
  } else if(trimmedCommand == '3'){

    //----------------------------
    //  TURN LEFT
    //----------------------------

    motorState = ROT_LEFT;
  } else if(trimmedCommand == '4') {

    //----------------------------
    //  TURN RIGHT
    //----------------------------

    motorState = ROT_RIGHT;
  }else if(trimmedCommand == '5'){

    //----------------------------
    //  DEPLOY AUGER
    //----------------------------

    augerState = DEPLOY;
  }else if(trimmedCommand == '6'){

    //----------------------------
    // UNDEPLOY AUGER
    //----------------------------

    augerState = UNDEPLOY;
  }else if(trimmedCommand == '7'){

    //----------------------------------
    //  START/STOP ROTATION OF DRILL
    //----------------------------------

    if (augerRotation == true){
      augerRotation = false;
    } else {
      augerRotation = true;
    }
  } else if(trimmedCommand == '8'){

    //----------------------------
    //  LOWER BOTTOM STEPPERS
    //----------------------------

    augerState = LOWER_BOT;
  } else if(trimmedCommand == '9'){

    //----------------------------
    //  RAISE BOTTOM STEPPERS
    //----------------------------

    augerState = RAISE_BOT;
  } else if(trimmedCommand == 'A'){

    //----------------------------
    //  LOWER TOP STEPPERS
    //----------------------------

    augerState = LOWER_TOP;
  } else if(trimmedCommand == 'B'){

    //----------------------------
    //  RAISE TOP STEPPERS
    //----------------------------

    augerState = RAISE_TOP;
  } else if(trimmedCommand == 'C'){

    //----------------------------
    //  EXTEND LINEAR ACTUATORS
    //----------------------------

    collectionState = EXTEND_ACT;
  } else if(trimmedCommand == 'D'){

    //----------------------------
    //  CONTRACT LINEAR ACTUATORS
    //----------------------------

    collectionState = CONTRACT_ACT;
  } else if(trimmedCommand == 'E'){

    //----------------------------
    //  ROTATE COLLECTION FAST
    //----------------------------

    collectionState = ROTATE_YEET;
  } else if(trimmedCommand == 'F'){

    //----------------------------
    //  ROTATE COLLECTION SLOW
    //----------------------------

    collectionState = ROTATE_ADJUST;
  }

  // Execute State Machines
  motorStates();
  augerStates();
  collectionStates();
}

//----------------------------------------------
//  MOTOR STATE MACHINE
//----------------------------------------------

void motorStates(){

  // Sets motor PWMs based on the direction of movement
  switch(motorState){
    case WAIT_M:
      leftPWMDesired = 1500;
      rightPWMDesired = 1500;
     break;
    case FORWARD:
      leftPWMDesired = 1700;
      rightPWMDesired = 1300;
      break;
    case BACKWARD:
      leftPWMDesired = 1300;
      rightPWMDesired = 1700;
      break;
    case ROT_LEFT:
      leftPWMDesired = 1300;
      rightPWMDesired = 1300;
      break;
    case ROT_RIGHT:
      leftPWMDesired = 1700;
      rightPWMDesired = 1700;
      break;
  }

  // Sends PWMs to the motors
  leftMotors.writeMicroseconds(leftPWMDesired);
  rightMotors.writeMicroseconds(rightPWMDesired); 

}


//----------------------------------------------------
//  MINING SYSTEM STATE MACHINE
//----------------------------------------------------

void augerStates(){
  switch(augerState){
    case WAIT_A:
      // If wait, send 100% duty cycle to both stages
      ledcWrite(PWMChannel_Upper, 8192);
      ledcWrite(PWMChannel_Lower, 8192);
      // Send Idle signal to rotational motor, with a value to keep auger upright
      augerDeploy.writeMicroseconds(1550);
      break;
    case DEPLOY:
      // Send value to allow auger to rotate upwards
      augerDeploy.writeMicroseconds(1850);
      break;
    case UNDEPLOY:
      // Send small backwards signal for auger adjustment
      augerDeploy.writeMicroseconds(1450);
      break;
    case LOWER_BOT:
      // Send 50% duty cycle to lower stage, 0 V to direction pin
      ledcWrite(PWMChannel_Lower, 4096);
      analogWrite(lowerStepperDirPin, 0);
      break;
    case RAISE_BOT:
      // Send 50% duty cycle to lower stage, 5 V to direction pin
      ledcWrite(PWMChannel_Lower, 4096);
      analogWrite(lowerStepperDirPin, 256);
      break;
    case LOWER_TOP:
      // Send 50% duty cycle to upper stage, 0 V to direction pin
      ledcWrite(PWMChannel_Upper, 4096);
      analogWrite(upperStepperDirPin, 0);
      break;
    case RAISE_TOP:
      // Sned 50% duty cycle to upper stage, 5 V to direction pin
      ledcWrite(PWMChannel_Upper, 4096);
      analogWrite(upperStepperDirPin, 256);
      break;
  }

  // Allows drill to rotate while the other states are functional
  if(augerRotation == true){
    augerSpin.writeMicroseconds(1600);
  } else {
    augerSpin.writeMicroseconds(1500);
  }
}

//-----------------------------------------------
//  COLLECTION SYSTEM STATE MACHINE
//-----------------------------------------------

void collectionStates(){
  switch(collectionState){
    case WAIT_C:
      // If Idle, send 0 signal to linear actuators and belt
      linearActuators.writeMicroseconds(1500);
      collectionSpin.writeMicroseconds(1500);
      break;
    case EXTEND_ACT:
      // Extend the linear actuators at full speed
      linearActuators.writeMicroseconds(1000);
      break;
    case CONTRACT_ACT:
      // Contract linear actuators at full speed
      linearActuators.writeMicroseconds(2000);
      break;
    case ROTATE_YEET:
      // Rotate belt fast for depositing
      collectionSpin.writeMicroseconds(1200);
      break;
    case ROTATE_ADJUST:
      // Rotate belt slow for small adjustments
      collectionSpin.writeMicroseconds(1400);
      break;
  }
}


