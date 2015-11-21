#define numPins 5
#include <PID_v1.h>

int inputPin[numPins] = {A1,A2,A3,A4,A5};
int outputPin[numPins] = {8,9,10,11,12};
int inputHighCount[numPins];
int inputTotalCount[numPins];
int lastInput[numPins];
double percentWidth[numPins];
bool inputHigh[numPins];
unsigned long pulseStart[numPins];
unsigned long pulseLength[numPins];
// PID Parameters
float kp = 0;
float ki = 0;
float kd = 0;
double pidSetpoint, pidInput, pidOutput; //just shit
PID myPID(&pidInput, &pidOutput, &pidSetpoint, kp, ki, kd, DIRECT); //sets up loop, input from encoder, output to ardupilot, setpoint is 0, DIRECT is because it's non-inverting

const int sampleRate = 10; //milliseconds sample raet


void setup() {
  for(int i = 0; i < numPins; i++){
    pinMode(inputPin[i], INPUT);
    pinMode(outputPin[i], OUTPUT);
    inputHighCount[i] = 0;
    inputTotalCount[i] = 0;
    lastInput[i] = LOW;
    percentWidth[i] = 0;
    inputHigh[i] = false;
    pulseStart[i] = 0;
    pulseLength[i] = 0;
  }

  pidSetpoint = 128; //because middle of 0-255

  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC); //turns that shit on
  myPID.SetSampleTime(sampleRate);
}

void loop() {
  unsigned long newTime = micros();
  for(int i = 0; i < numPins; i++){
    //Read Input
    int newInput = digitalRead(inputPin[i]);
    if(newInput == HIGH && lastInput[i] == LOW){
      percentWidth[i] = (double)inputHighCount[i]/inputTotalCount[i];
      pulseLength[i] = newTime - pulseStart[i];
      pulseStart[i] = newTime;
      inputTotalCount[i] = 0;
      inputHighCount[i] = 0;
    }
    if(newInput == HIGH)
      inputHighCount[i]++;
    inputTotalCount[i]++;
    lastInput[i] = newInput;
    
    //Write Output
    unsigned long pulseTime = newTime - pulseStart[i];
    
    if(pulseTime == 0 && !inputHigh[i]){
      digitalWrite(outputPin[i], HIGH);
      inputHigh[i] = true;
    }
    if(pulseTime > pulseLength[i] * percentWidth[i] && inputHigh[i]){
      digitalWrite(outputPin[i], LOW);
      inputHigh[i] = false;
    } 
  }
  encoder = analogRead(whatever_fucking_pin);
  pidInput = map(encoder, 0, 200, 0, 255); //mappind encoder input from 0-200 to right scale
  myPID.Compute();
  //now the PID loop has run, and if you call pidOutput, you'll get a value from 0 to 255 that you can do whatever to

}
