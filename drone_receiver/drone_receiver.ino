/* PIN(I/O)  ARRAY  COLOR
 * A1/8      0      WHITE
 * A2/9      1      BLUE
 * A3/10     2      YELLOW
 * A4/11     3      GREEN
 * A5/12     4      ORANGE
 */
#define numPins 5
 
int inputPin[numPins] = {A1,A2,A3,A4,A5};
int outputPin[numPins] = {8,9,10,11,12};
int inputHighCount[numPins];
int inputTotalCount[numPins];
int lastInput[numPins];
double percentWidth[numPins];
bool inputHigh[numPins];
unsigned long pulseStart[numPins];
unsigned long pulseLength[numPins];

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
  
  Serial.begin(9600);
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
}
