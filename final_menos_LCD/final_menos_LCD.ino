//Line Following Setup
#include <Servo.h>                           // Use the Servo library (included with Arduino IDE)  
#include <SoftwareSerial.h>
Servo servoL;                                // Define the left and right servos
Servo servoR;

int courtSports = -1;
int weightLifting = -1;
int cycling = -1;
int diving = -1;
int basketball = -1;
int sum = -1;
String answer;

#define Rx 10 // DOUT to pin 10
#define Tx 11 // DIN to pin 11


SoftwareSerial Xbee (Rx, Tx);


const int TxPin = 9;
const int sio = 53;            // ColorPAL connected to pin 2
const int unused = 255;         // Non-existant pin # for SoftwareSerial
const int sioBaud = 4800;
const int waitDelay = 200;


// Received RGB values from ColorPAL
int red;
int grn;
int blu;
int farLP = 8;
int centerLP = 6;
int centerRP = 5;
int farRP = 4;
int left = 0;
int right = 0;
char medal;
int led = 3; // this may vary based on your hardware configuration.
char outgoing = 0;
boolean linefollowing = true;
boolean senselcd = false;
boolean comm = false;
boolean donereceiving = false;
boolean dis=false;


// Set up two software serials on the same pin.
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);
SoftwareSerial serin(sio, unused);
SoftwareSerial serout(unused, sio);

void setup() {
  Serial.begin(9600);
  servoL.attach(13);                         // Attach (programmatically connect) servos to pins on Arduino
  servoR.attach(12);

}


long RCTime(int sensorIn) {
  long duration = 0;
  pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
  digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
  delay(1);                      // Wait 1ms
  pinMode(sensorIn, INPUT);      // Make pin INPUT
  digitalWrite(sensorIn, LOW);   // Turn off internal pullups
  while (digitalRead(sensorIn)) { // Wait for pin to go LOW
    duration++;
  }
  return duration;
}

int isHighfR(int rctime) {
  if (rctime < 40)
    return 0;
  else
    return 1;
}

int isHighCR(int rctime) {
  if (rctime < 35)
    return 0;
  else
    return 1;
}

int isHighCL(int rctime) {
  if (rctime < 25)
    return 0;
  else
    return 1;
}

int isHighfL(int rctime) {
  if (rctime < 20)
    return 0;
  else
    return 1;
}

void loop() {
  while (linefollowing == true) {
    linefollow();
  }
  resetsetup();
  LCDsetup();
  while (senselcd == true) {
    readData();
  }
  communication();
  while (comm == true) {
    output();
    scores();
  }
  while(dis==true){
  disp();
  }
}

// Reset ColorPAL; see ColorPAL documentation for sequence
void reset() {
  delay(200);
  pinMode(sio, OUTPUT);
  digitalWrite(sio, LOW);
  pinMode(sio, INPUT);
  while (digitalRead(sio) != HIGH);
  pinMode(sio, OUTPUT);
  digitalWrite(sio, LOW);
  delay(80);
  pinMode(sio, INPUT);
  delay(waitDelay);
}

void readData() {
  Serial.println("ReadData");
  char buffer[32];

  if (serin.available() > 0) {
    // Wait for a $ character, then read three 3 digit hex numbers
    buffer[0] = serin.read();
    if (buffer[0] == '$') {
      for (int i = 0; i < 9; i++) {
        while (serin.available() == 0);     // Wait for next input character
        buffer[i] = serin.read();
        if (buffer[i] == '$')               // Return early if $ character encountered
          return;
      }
      parseAndPrint(buffer);
      delay(50);
    }
  }
}


void linefollow() {

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
  //Serial.println(pins);

  // Steering based on state of the four QTI sensors
  int vL, vR;
  switch (pins)                              // Compare pins to known line following states
  {
    case 1000:
      vL = -200;                             // -100 to 100 indicate course correction values
      vR = 200;                              // -100: full reverse; 0=stopped; 100=full forward
      left = left + 1;
      break;
    case 1100:
      vL = -200;
      vR = 0;
      left = left + 1;
      break;
    case 101:
      vL = -200;
      vR = -200;
      left + 1;
      right = right + 1;
    case 1010:
      vL = 200;
      vR = 200;
    case 100:
      vL = -200;
      vR = 200;
      left = left + 1;
      break;
    case 110:
      vL = 100;
      vR = 100;
      break;
    case 10:
      vL = 200;
      vR = -200;
      right = right + 1;
      break;
    case 11:
      vL = 0;
      vR = -200;
      right = right + 1;
      break;
    case 1:
      vL = 200;
      vR = -200;
      right = right + 1;
      break;
    case 1110:
      vL = -200;
      vR = 200;
      left = left + 1;
      break;
    case 111:
      vL = 200;
      vR = -200;
      right = right + 1;
      break;
    case 1111:
      servoL.writeMicroseconds(1500);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500);
      delay(250);


      if (right > left) {
        servoL.writeMicroseconds(1300);
        servoR.writeMicroseconds(1700);
        delay(240);
        servoL.writeMicroseconds(1500);
        servoR.writeMicroseconds(1500);
        servoL.writeMicroseconds(1700);
        delay(1300);
        servoL.writeMicroseconds(1500);

      }
      else if (left > right) {
        servoL.writeMicroseconds(1300);
        servoR.writeMicroseconds(1700);
        delay(240);
        servoL.writeMicroseconds(1500);
        servoR.writeMicroseconds(1500);

        servoR.writeMicroseconds(1300);
        delay(1310);
        servoR.writeMicroseconds(1500);
      }
      servoL.writeMicroseconds(1700);
      servoR.writeMicroseconds(1300);
      delay(600);
      servoL.writeMicroseconds(1500);
      servoR.writeMicroseconds(1500);
      servoL.detach();
      servoR.detach();
      linefollowing = false;
      senselcd = true;
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

// Parse the hex data into integers
void parseAndPrint(char * data) {
  sscanf (data, "%3x%3x%3x", &red, &grn, &blu);
  char buffer[32];



  medal = 0;
  if (red > 20 ) {
    medal = 'g';                         // Required delay
    answer="Gold";
  }
  else  if (blu > 20) {
    medal = 's';        
    answer="Silver";// Required delay
  }
  else if (red < 20 && blu < 20 && grn < 20) {
    medal = 'b';
  answer="Bronze";
  }
  senselcd = false;
  comm = true;
}

void resetsetup() {
  reset();                  // Send reset to ColorPal
  serout.begin(sioBaud);
  pinMode(sio, OUTPUT);
  serout.print("= (00 $ m) !"); // Loop print values, see ColorPAL documentation
  serout.end();              // Discontinue serial port for transmitting
}
void LCDsetup() {
  serin.begin(sioBaud);            // Set up serial port for receiving
  pinMode(sio, INPUT);
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
  mySerial.begin(9600);
}

void communication() {
  Serial.begin(9600); // Set to No line ending;
  Xbee.begin(9600); // type a char, then hit enter
  delay(500); // wait half a second
  pinMode(led, OUTPUT);

}

void output() {
  if (medal == 'g') { // Is the button pushed?
    outgoing = 'g'; // sets outgoing character to s
    diving = 'g';
    Serial.println(outgoing);
    Xbee.print(outgoing);
    delay(500);
  }
  else if ( medal == 's') {
    outgoing = 'h'; // sets outgoing character to s
    diving = 'h';
    Serial.println(outgoing);
    Xbee.print(outgoing);
    delay(500);
  }
  else if ( medal == 'b') {
    outgoing = 'i'; // sets outgoing character to s
    diving = 'i';
    Serial.println(outgoing);
    Xbee.print(outgoing);
    delay(200);
  }
}
void scores() {
if(Xbee.available()) { // Is data available from XBee?
    char incoming = Xbee.read(); // Read character,
    Serial.println(incoming); // send to Serial Monitor
if(incoming == 'a')
    {
      courtSports = 2;
    }
    if(incoming == 'b')
    {
      courtSports = 1;
    }
    if(incoming == 'c')
    {
      courtSports = 0;
    }
     if (incoming == 'd') {
      cycling = 2;
    }
    if (incoming == 'e') {
      cycling = 1;
    }
    if (incoming == 'f') {
      cycling = 0;
    }
    if (medal == 'g') {
      diving = 2;
    }
    if (medal == 's') {
      diving = 1;
    }
    if (medal == 'b') {
      diving = 0;
    }
    if(incoming == 'j') 
    {
      weightLifting = 2; //CHANGE IT LATER
    }
    if(incoming == 'k')
    {
      weightLifting = 1;
    }
    if(incoming == 'l')
    {
      weightLifting = 0;
    }
     if (incoming == 'm') {
      basketball = 2;
    }
    if (incoming == 'n') {
      basketball = 1;
    }
    if (incoming == 'o') {
      basketball = 0;
    }
    /*
    if(incoming == 'F')
    {
      Serial.println("FFFFFF"); // send to Serial Monitor
    }*/
 
   
  //if(weightLifting > -1 && courtSports > -1 && cycling > -1 && diving > -1 && basketball > -1)
 
if(diving>-1 && courtSports>-1 && basketball > -1 &&weightLifting > -1 ){ 
    //sum = courtSports + weightLifting+ cycling+diving+ basketball;
    sum=diving+basketball+courtSports+weightLifting;
    sum = (sum%3); //WORK ON DISPLAYING THE SUM VARIABLE ON THE LCD DISPLAY
    Serial.println("SUM: ");
    Serial.println(sum);
    comm = false;
    dis=true;
  }
  
}

}
void disp(){
  mySerial.write(12);                 // Clear
    mySerial.write(17);                 // Turn backlight on
    delay(5);  
    mySerial.print(answer);// Required delay
    mySerial.print("  Sum= ");
    mySerial.print(sum);  // First line
    mySerial.write(13);                  // Form feed
    delay(10000);                        // Wait 3 seconds
    mySerial.write(18);
  int count = 0;
  while(count < 2000) {
  if (sum==0){
    Xbee.print('0');
  }
  else if (sum==1){
    Xbee.print('1');
  }
  else if (sum == 2) {
    Xbee.print('2');
  }
  Serial.print(sum);
  count=count+1;
  }
  dis=false;
}

/*void celebration() {
  if (sum==0){
    dance();
  }
  else if (sum==1){
    lightshow();
  }
  else if (sum == 2) {
    anthem();
  }
}
*/

