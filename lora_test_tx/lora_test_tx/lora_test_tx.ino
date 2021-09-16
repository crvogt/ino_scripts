void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(9600);
}

void loop(){
  
  delay(100);
  Serial.write("1");
  digitalWrite(13, HIGH);
  delay(300);
  Serial.write("0");
  digitalWrite(13, LOW);
}
