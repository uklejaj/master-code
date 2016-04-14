#include <Servo.h>                           // Use the Servo library (included with Arduino IDE)  

Servo servoL;                                // Define the left and right servos
Servo servoR;

int farLP = 6;
int centerLP = 9;
int centerRP = 5;
int farRP = 4;



// Perform these steps with the Arduino is first powered on
void setup()
{
  Serial.begin(9600);                        // Set up Arduino Serial Monitor at 9600 baud
  servoL.attach(13);                         // Attach (programmatically connect) servos to pins on Arduino
  servoR.attach(12);
  
}

long RCTime(int sensorIn){
   long duration = 0;
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   delay(1);                      // Wait 1ms
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   while(digitalRead(sensorIn)){  // Wait for pin to go LOW
      duration++;
   }
   return duration;
}

int isHighfR(int rctime) {
  if(rctime < 10000)
    return 0;
  else
    return 1;
}

int isHighCR(int rctime) {
  if(rctime < 10000)
    return 0;
  else
    return 1;
}

int isHighCL(int rctime) {
  if(rctime < 8000)
    return 0;
  else
    return 1;
}

int isHighfL(int rctime) {
  if(rctime < 8000 )
    return 0;
  else
    return 1;
}

// This code repeats indefinitely
void loop()
{
  pinMode(farLP, OUTPUT);
  pinMode(centerLP, OUTPUT);
  pinMode(centerRP, OUTPUT);
  pinMode(farRP, OUTPUT);
  
  digitalWrite(farLP, HIGH);
  digitalWrite(centerLP, HIGH);
  digitalWrite(centerRP, HIGH);
  digitalWrite(farRP, HIGH);
  delayMicroseconds(230); // allow capcitor charge in QTI

  pinMode(farLP, INPUT);
  pinMode(centerLP, INPUT);
  pinMode(centerRP, INPUT);
  pinMode(farRP, INPUT);

  digitalWrite(farLP, LOW);
  digitalWrite(centerLP, LOW);
  digitalWrite(centerRP, LOW);
  digitalWrite(farRP, LOW);

  delayMicroseconds(230);

  int pin4 = isHighfR(RCTime(farRP));
  int pin1 = isHighCR(RCTime(centerRP));
  int pin2 = isHighCL(RCTime(centerLP));
  int pin3 = isHighfL(RCTime(farLP)); 
 // Serial.println(RCTime(farRP), RCTime(farLP));
  //Serial.println(isHighfR(RCTime(farRP)), isHighfL(RCTime(farLP)));
  String s = String(pin3) + String(pin2) + String(pin1) + String(pin4);
  int pins = s.toInt();
  Serial.println(pins);

  int myPinVals[] = {pin3, pin2, pin1, pin4};
 
  // Determine how to steer based on state of the four QTI sensors
  int vL, vR;
  switch(pins)                               // Compare pins to known line following states
  {
    case 1000:                        
      vL = -200;                             // -100 to 100 indicate course correction values
      vR = 200;                              // -100: full reverse; 0=stopped; 100=full forward
      break;
    case 1100:                        
      vL = 0;
      vR = 200;
      break;
    case 100:                        
      vL = 100;
      vR = 200;
      break;
    case 110:                        
      vL = 200;
      vR = 200;
      break;
    case 10:                        
      vL = 200;
      vR = 100;
      break;
    case 11:                        
      vL = 200;
      vR = 0;
      break;
      case 0000:
      vL=0;
      vR=0;
      break;
    case 1:                        
      vL = 200;
      vR = -200;
      break;
    case 101:
       vL = -200;
      vR = 200;
      break;
    case 1010:
      vL = 200;
      vR = -200;
      break;
    case 1110:
      vL = 200;
      vR = -200;
      break;
      case 1101:
      vL=0;
      vR=0;
      break;
       case 1011:
      vL=0;
      vR=0;
      break;
    case 111: 
      vL = -200;
      vR = 200;
      break;

    case 1111:
      servoL.writeMicroseconds(1500);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500);
      delay(200);
      servoL.writeMicroseconds(1500 + 100);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - 100);
      break;
  }
  
  servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
  servoR.writeMicroseconds(1500 - vR);
  
  delay(50);                                // Delay for 50 milliseconds (1/20 second)
}
