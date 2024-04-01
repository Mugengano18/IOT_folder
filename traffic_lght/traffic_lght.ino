

void setup() {
  // put your setup code here, to run once:

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  tone(10, 1000); 
  delay(1000);     
  noTone(10);     // Stop sound...
  delay(1000);
  delay(10000);
  digitalWrite(13,LOW);
   


  digitalWrite(11,HIGH);
  delay(1000);
  digitalWrite(11,LOW);
  delay(500);

  digitalWrite(12,HIGH);
  delay(5000);
  digitalWrite(12,LOW);
  delay(500);




}
