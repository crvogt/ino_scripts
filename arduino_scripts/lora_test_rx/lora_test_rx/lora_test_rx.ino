void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(9600);
}

void loop(){
  if(Serial.available()){
    char data_rx = Serial.read();

    if(data_rx == '1')
      digitalWrite(13, HIGH);
    if(data_rx =='0')
      digitalWrite(13, LOW);
  }
}
