#define stepperPulsePin 23
#define stepperDirPin 5

int dutyCycle = 128;
const int PWMChannel = 0;
const int PWMResolution = 13;
const int PWMFreq = 1000;

void setup() {
  //Serial.println("Welcome to the stepper motor tester");
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  ledcAttachPin(stepperPulsePin, PWMChannel);
}

void loop() {
  ledcWrite(PWMChannel, 4096);
  analogWrite(stepperDirPin, 0);

}
