#include <SD.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>

// LED
#define INDICATOR_PIN 9
// Pressure Sensor
#define PRESSURE_PIN A0

/******ADXL*********/
ADXL345 adxl = ADXL345(5);
int g_range = 16;
int adxl_frequency = 3200;
int z_threshold = 48;

// Record for one minute
long int record_time = 60000;
long int lstart_time = 0;

/********SD Card*****/
const int chip_select = 10;
File data_file;

void setup(){
  Serial.begin(9600);
  // Our LED to tell us when we're recording
  pinMode(INDICATOR_PIN, OUTPUT);
    
  // Turn on and set ADXL
  adxl.powerOn();
  adxl.setRangeSetting(g_range);
  adxl.setSpiBit(0);
  adxl.setRate(adxl_frequency);

  // Setup SD card
  if(!SD.begin(chip_select)){
    Serial.println("Unable to find SD card");
    while(1){}
  }
  else{
    Serial.println("FOUND SD CARD");
  }
  // Create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      data_file = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
}

void loop(){
  int x, y, z;
  adxl.readAccel(&x, &y, &z);

  if(z > z_threshold){
    digitalWrite(INDICATOR_PIN, HIGH);
    while(lstart_time - millis() < record_time){
      data_file.print(x);
      data_file.print(", ");
      data_file.print(y);
      data_file.print(", ");
      data_file.print(z);
      data_file.print(", ");
      data_file.println(analogRead(PRESSURE_PIN));
      delayMicroseconds(10);
    }
  }
  else{   
    digitalWrite(INDICATOR_PIN, LOW);
  }
}
