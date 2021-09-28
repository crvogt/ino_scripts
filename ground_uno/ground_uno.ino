const int toggle_read_pin = 7;

void setup(){
  Serial.begin(9600);
  pinMode(toggle_read_pin, INPUT);
}

void loop(){
  if(digitalRead(toggle_read_pin)){
    Serial.println("Toggled");
  }
  else{
    Serial.println("Un-Toggled");
  }
}
