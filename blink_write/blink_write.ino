const int writePin = 5;

void setup(){
  Serial.begin(9600);
  pinMode(writePin, OUTPUT);
}

void loop(){
  Serial.write('0');
  delay(1000);
  Serial.write('r');
  delay(1000);
}
