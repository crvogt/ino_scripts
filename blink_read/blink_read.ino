const int ledPin = 7;
const int readPin = 8;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  if(Serial.available()){
    if(Serial.read() == 'r'){
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else{
      digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
    }
  }
}
