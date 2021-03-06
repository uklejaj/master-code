
#include <SoftwareSerial.h>


const int TxPin = 6;
const int sio = 53;            // ColorPAL connected to pin 2
const int unused = 255;         // Non-existant pin # for SoftwareSerial
const int sioBaud = 4800;
const int waitDelay = 200;


// Received RGB values from ColorPAL
int red;
int grn;
int blu;
int counter=0;
boolean senselcd=true;

// Set up two software serials on the same pin.
SoftwareSerial mySerial = SoftwareSerial(255, TxPin);
SoftwareSerial serin(sio, unused);
SoftwareSerial serout(unused, sio);

void setup() {
  Serial.begin(9600);
  reset();                  // Send reset to ColorPal
  serout.begin(sioBaud);
  pinMode(sio, OUTPUT);
  serout.print("= (00 $ m) !"); // Loop print values, see ColorPAL documentation
  serout.end();              // Discontinue serial port for transmitting

  serin.begin(sioBaud);            // Set up serial port for receiving
  pinMode(sio, INPUT);
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);

   pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  while(senselcd==true){
    readData();
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
  char buffer[32];
 
  if (serin.available() > 0) {
    // Wait for a $ character, then read three 3 digit hex numbers
    buffer[0] = serin.read();
    if (buffer[0] == '$') {
      for(int i = 0; i < 9; i++) {
        while (serin.available() == 0);     // Wait for next input character
        buffer[i] = serin.read();
        if (buffer[i] == '$')               // Return early if $ character encountered
          return;
      }
      parseAndPrint(buffer);
      delay(300);
    }
  }
}

// Parse the hex data into integers
void parseAndPrint(char * data) {
  sscanf (data, "%3x%3x%3x", &red, &grn, &blu);
  char buffer[32];

 if(counter==5){
if (red > 15 ){
  mySerial.write(12);                 // Clear             
  mySerial.write(17);                 // Turn backlight on
  delay(5);                           // Required delay
  mySerial.print("Breathtaking swan dive!");  // First line
  mySerial.write(13);                 // Form feed
  mySerial.print("GOLD!");             // Second line
  delay(3000);                        // Wait 3 seconds
  mySerial.write(18); 
}
else  if (blu > 15){
    mySerial.write(12);                 // Clear             
  mySerial.write(17);                 // Turn backlight on
  delay(5);                           // Required delay
  mySerial.print("Incredible pencil dive!");  // First line
  mySerial.write(13);                 // Form feed
  mySerial.print("SILVER!");             // Second line
  delay(3000);                        // Wait 3 seconds
  mySerial.write(18); 
}
  else if(red < 15 && blu < 15 && grn < 15){
 mySerial.write(12);                 // Clear             
  mySerial.write(17);                 // Turn backlight on
  delay(5);                           // Required delay
  mySerial.print("Subpar cannonball...");  // First line
  mySerial.write(13);                 // Form feed
  mySerial.print("Fine... bronze...");             // Second line
  delay(3000);                        // Wait 3 seconds
  mySerial.write(18); 
  }
}
  if(counter!=5){
  counter=counter+1;
  }
  else{
    senselcd=false;
  }
}


