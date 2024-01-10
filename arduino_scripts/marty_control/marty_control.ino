#include <Servo.h>

Servo eyebrows;

void angry_face(Servo _servo){
  int pos = 956;
  _servo.writeMicroseconds(pos);
  Serial.println(_servo.read());
  Serial.println(_servo.readMicroseconds());
}

void setup(){
  Serial.begin(9600);
  eyebrows.attach(12);
}

void loop(){
  // Receive signal from higher level computer
  angry_face(eyebrows);
  while(1){}
} 
