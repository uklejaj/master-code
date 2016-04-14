void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
}

void loop() {
  delay(500);
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  delay(500);
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
  delay(500);
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  delay(500);
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
  delay(500);
  
}
