/*
   Credits: Joyce Choi
*/

#include <SoftwareSerial.h>
#define Rx 10 // DOUT to pin 10
#define Tx 11 // DIN to pin 11
int led = 3; // this may vary based on your hardware configuration.
char medal;
  char outgoing = 0;

SoftwareSerial Xbee (Rx, Tx);
void communication() {
  Serial.begin(9600); // Set to No line ending;
  Xbee.begin(9600); // type a char, then hit enter
  delay(500); // wait half a second
  pinMode(led, OUTPUT);
  
}
void output() {
  if (medal = 'g') { // Is the button pushed?
    outgoing = 'g'; // sets outgoing character to s
    Xbee.print(outgoing);
    delay(500);
  }
  else if ( medal == 's') {
     outgoing = 'h'; // sets outgoing character to s
    Xbee.print(outgoing);
    delay(500);
  }
  else if ( medal == 'b') {
    outgoing = 'i'; // sets outgoing character to s
    Xbee.print(outgoing);
    delay(500);
  }
}
void setup() {
  communication();
}
void loop() {
  output();

  if (Xbee.available()) { // Is data available from XBee?
    char incoming = Xbee.read(); // Read character,
    Serial.println(incoming); // send to Serial Monitor
    if (incoming) {
      digitalWrite(led, HIGH); // switches on the light if signal is received
      delay(500); // waits half a second
      digitalWrite(led, LOW); // turns off LED
    }
  }
  delay(50);
}

