int throttlePin = 8;
int throttleOut = 9;
int throttleHighCount = 0;
int throttleCount = 0;
int throttle = 0;
int temp = 0;
double percent = 0;
bool throttleHigh = false;
unsigned long pulseStart = 0;
unsigned long pulseLength = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(throttlePin, INPUT);
  pinMode(throttleOut, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long newTime = micros();
  
  //Read Input
  int newThrottle = digitalRead(throttlePin);
  if(newThrottle == HIGH && throttle == LOW){
    percent = (double)throttleHighCount/throttleCount;
    pulseLength = newTime - pulseStart;
    pulseStart = micros();
    //temp++;
    //if(temp%10 == 0)
    //  Serial.println(percent*100, 4);
    throttleCount = 0;
    throttleHighCount = 0;
  }
  if(newThrottle == HIGH)
    throttleHighCount++;
  throttleCount++;
  throttle = newThrottle;
  
  //Write Output
  unsigned long pulseTime = newTime - pulseStart;
  if(pulseTime == 0 && !throttleHigh){
    digitalWrite(throttleOut, HIGH);
    throttleHigh = true;
  }
  if(pulseTime > pulseLength * percent && throttleHigh){
    digitalWrite(throttleOut, LOW);
    throttleHigh = false;
  }
  delayMicroseconds(2);
}
