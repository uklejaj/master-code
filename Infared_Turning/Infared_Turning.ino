/*
 * Robotics with the BOE Shield – TestBothIrAndInciators
 * Test both IR detection circuits with the Serial Monitor.  Displays 1 if
 * the left IR detector does not detect an object, or 0 if it does.
 * Also displays IR detector states with indicator LEDs.
 */

void setup()                                 // Built-in initialization block
{
  tone(4, 3000, 1000);                       // Play tone for 1 second
  delay(1000);                               // Delay to finish tone

  pinMode(10, INPUT);  pinMode(9, OUTPUT);   // Left IR LED & Receiver
  pinMode(3, INPUT);  pinMode(2, OUTPUT);    // Right IR LED & Receiver
  pinMode(8, OUTPUT); pinMode(7, OUTPUT); // Indicator LEDs
pinMode(6,OUTPUT); pinMode(5,OUTPUT);
  Serial.begin(9600);                        // Set data rate to 9600 bps
}  
 
void loop()                                  // Main loop auto-repeats
{
  int irLeft = irDetect(9, 10, 38000);       // Check for object on left
  int irRight = irDetect(2, 3, 38000);       // Check for object on right

  digitalWrite(8, irLeft);                  // LED states opposite of IR   
  digitalWrite(7, !irLeft);
  digitalWrite(6, !irRight);
  digitalWrite(5, irRight);
 

  Serial.print(irLeft);                      // Display 1/0 no detect/detect
  Serial.print("  ");                        // Display 1/0 no detect/detect
  Serial.println(irRight);                   // Display 1/0 no detect/detect

  delay(100);                                // 0.1 second delay
String s= String(irLeft)+String(irRight);
int detection = s.toInt();
Serial.println(detection);

switch(detection){
case 11:
pinMode(8,HIGH);
pinMode(5,HIGH);
delay(2000);
pinMode(8,LOW);
pinMode(5,LOW);
break;

case 10:
pinMode(8,HIGH);
pinMode(6,HIGH);
delay(2000);
pinMode(8,LOW);
pinMode(6,LOW);
break;

case 01:
pinMode(7,HIGH);
pinMode(5,HIGH);
delay(2000);
pinMode(7,LOW);
pinMode(5,LOW);
break;

case 00:
pinMode(7,HIGH);
pinMode(6,HIGH);
delay(2000);
pinMode(7,LOW);
pinMode(6,LOW);
break;

}
}

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  


