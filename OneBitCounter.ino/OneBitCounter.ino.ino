void setup() {
  pinMode(2,OUTPUT);
  
  digitalWrite(2,LOW);
}

void loop() {
  delay(500);
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  
}
