/*
   Credits: Joyce Choi
*/

#include <SoftwareSerial.h>
#define Rx 10 // DOUT to pin 10
#define Tx 11 // DIN to pin 11
int led = 3; // this may vary based on your hardware configuration.
int button = 7; // this may vary based on your hardware configuration.

SoftwareSerial Xbee (Rx, Tx);

void setup() {
  Serial.begin(9600); // Set to No line ending;
  Xbee.begin(9600); // type a char, then hit enter
  delay(500); // wait half a second
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
}
void loop() {
  int pushed = 0; // the button is *not* pushed by default
  pushed = digitalRead(button);
  if (pushed == HIGH) { // Is the button pushed?
    char outgoing = 's'; // sets outgoing character to s
    Xbee.print(outgoing);
    delay(200);
  }
  if (Xbee.available()) { // Is data a vailable from XBee?
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
