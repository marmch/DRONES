int throttlePin = 8;
int throttleHighCount = 0;
int throttleCount = 0;
int throttle = 0;
int temp = 0;

void writePWM(double percentWidth, int pin){
  Serial.println(percentWidth);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(throttlePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int newThrottle = digitalRead(throttlePin);
  if(newThrottle == HIGH && throttle == LOW){
    double percent = (double)throttleHighCount/throttleCount;
    //writePWM(percent, throttlePin);
    temp++;
    if(temp%10 == 0)
      Serial.println(percent*100, 4);
    throttleCount = 0;
    throttleHighCount = 0;
  }
  if(newThrottle == HIGH)
    throttleHighCount++;
  throttleCount++;
  throttle = newThrottle;
  delayMicroseconds(5);
}
