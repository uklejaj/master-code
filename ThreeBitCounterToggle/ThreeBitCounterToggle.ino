void setup() {
  pinMode(2, OUTPUT);  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  }

 void loop() {
  delay(500);
  digitalWrite(2, !digitalRead(2));
  delay(500);
  digitalWrite(2, !digitalRead(2));
  digitalWrite(3,!digitalRead(3));
  delay(500);
  digitalWrite(2,!digitalRead(2));
  delay(500);
  digitalWrite(2,!digitalRead(2));
  digitalWrite(3,!digitalRead(3));
  digitalWrite(4,!digitalRead(4));
  delay(500);
  digitalWrite(2, !digitalRead(2));
  delay(500);
  digitalWrite(2, !digitalRead(2));
  digitalWrite(3,!digitalRead(3));
  delay(500);
  digitalWrite(2,!digitalRead(2));
  delay(500);
  digitalWrite(2,!digitalRead(2));
  digitalWrite(3,!digitalRead(3));
  digitalWrite(4,!digitalRead(4));
  
  
 }

