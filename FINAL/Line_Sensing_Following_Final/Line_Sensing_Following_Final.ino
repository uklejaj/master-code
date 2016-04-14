                                                                                                                                  //Line Following Setup
#include <Servo.h>                           // Use the Servo library (included with Arduino IDE)  
#include <SoftwareSerial.h>
Servo servoL;                                // Define the left and right servos
Servo servoR;

int farLP = 9;
int centerLP = 6;
int centerRP = 5;
int farRP = 4;
int left=0;
int right=0;
boolean linefollowing=true;
boolean sensing=true;
                                                                                                                                //End of Color Setup
const int sio = 53;            // ColorPAL connected to pin 2
const int unused = 255;         // Non-existant pin # for SoftwareSerial
const int sioBaud = 4800;
const int waitDelay = 200;
const int greenPin = 2;
const int redPin = 8;

 // Received RGB values from ColorPAL
int red;
int grn;
int blu;

 // Set up two software serials on the same pin.
SoftwareSerial serin(sio, unused);
SoftwareSerial serout(unused, sio);
                               
                                                                                                                                                                                                                             //End of Communication Setup

                                                                                                                                 

void setup()
{
  Serial.begin(9600);                        // Set up Arduino Serial Monitor at 9600 baud
  servoL.attach(13);                         // Attach (programmatically connect) servos to pins on Arduino
  servoR.attach(12);
//    reset();                  // Send reset to ColorPal
//  serout.begin(sioBaud);
//  pinMode(sio, OUTPUT);
//  serout.print("= (00 $ m) !"); // Loop print values, see ColorPAL documentation
//  serout.end();              // Discontinue serial port for transmitting
//
//  serin.begin(sioBaud);            // Set up serial port for receiving
//  pinMode(sio, INPUT);
//  pinMode(2, OUTPUT);
//  pinMode(8, OUTPUT);
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
  if(rctime < 40)
    return 0;
  else
    return 1;
}

int isHighCR(int rctime) {
  if(rctime < 35)
    return 0;
  else
    return 1;
}

int isHighCL(int rctime) {
  if(rctime < 25)
    return 0;
  else
    return 1;
}

int isHighfL(int rctime) {
  if(rctime < 20)
    return 0;
  else
    return 1;
}

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
  //Serial.println(RCTime(centerLP));
  //Serial.println(isHighCR(RCTime(farRP)));
  String s = String(pin3) + String(pin2) + String(pin1) + String(pin4);
  int pins = s.toInt();
  Serial.println(pins);
 
  // Steering based on state of the four QTI sensors
  int vL, vR;
  switch(pins)                               // Compare pins to known line following states
  {
    case 1000:                        
      vL = -200;                             // -100 to 100 indicate course correction values
      vR = 200;                              // -100: full reverse; 0=stopped; 100=full forward
      left=left+1;
      break;
    case 1100:                        
      vL = -200;
      vR = 0;
      left=left+1;
      break;
    case 101:
      vL = -200;
      vR = -200;
      left+1;
      right=right+1;
    case 1010:
      vL = 200;
      vR = 200;
    case 100:                        
      vL = -200;
      vR = 200;
      left=left+1;
      break;
    case 110:                        
      vL = 100;
      vR = 100;
      break;
    case 10:                        
      vL = 200;
      vR = -200;
      right=right+1;
      break;
    case 11:                        
      vL = 0;
      vR = -200;
      right=right+1;
      break;
    case 1:                        
      vL = 200;
      vR = -200;
      right=right+1;
      break;
    case 1110:
      vL = -200;
      vR = 200;
      left=left+1;
      break;
    case 111: 
      vL = 200;
      vR = -200;
      right=right+1;
      break;
    case 1111:
      servoL.writeMicroseconds(1500);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500);
      delay(250);
      Serial.println(left);
      Serial.println(right);
    servoL.writeMicroseconds(1300);
    servoR.writeMicroseconds(1700);
    delay(300);
    servoL.writeMicroseconds(1500);
    servoR.writeMicroseconds(1500);
      
if(right>left){
    servoL.writeMicroseconds(1700);
    delay(1915);
    servoL.writeMicroseconds(1500);
 
} 
else if(left>right){
    servoR.writeMicroseconds(1300);
    delay(1915);
    servoR.writeMicroseconds(1500);
}
servoL.writeMicroseconds(1700);
    servoR.writeMicroseconds(1300);
    delay(1000);
    servoL.writeMicroseconds(1500);
    servoR.writeMicroseconds(1500);
    
      break;
    case 0000:
      vL = 0;
      vR = 0; 
      break;
  
  }
  
  servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
  servoR.writeMicroseconds(1500 - vR);
  
  
                            // Delay for 50 milliseconds (1/20 second)

delay(50); 

  }  
  



                                                                                                                          
