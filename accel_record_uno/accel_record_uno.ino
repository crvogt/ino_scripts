/*
 * Uno to communicate with ground station and save
 * pressure and temperature data as well as send 
 * to ground uno.
 * Triggers accelerometer recording on sibling uno
 */
#include <SD.h>
#include <SparkFun_ADXL345.h>
#include <Wire.h>

/*******ADXL********/
ADXL345 adxl = ADXL345(5);
long int lstart_time = 0;
// Record for two minutes
long int record_time = 120000;

/********SD Card*****/
const int chip_select = 10;
File data_file;

void setup(){
  Serial.begin(9600);

  // Turn on ADXL
  adxl.powerOn();
  adxl.setRangeSetting(16);
  adxl.setSpiBit(0);
  adxl.setRate(3200);

  // Setup SD card
  if(!SD.begin(chip_select)){
    Serial.println("Unable to find SD card");
    while(1){}
  }
  else{
    Serial.println("FOUND SD CARD");
  }
  while(1){}
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
  if(Serial.available()){
    if(Serial.read() == 'r'){
      Serial.write('w');
      while(lstart_time - millis() < record_time){
        int x, y, z;
        adxl.readAccel(&x, &y, &z);
        data_file.print(x);
        data_file.print(", ");
        data_file.print(y);
        data_file.print(", ");
        data_file.print(z);
        data_file.print("\n");
        delayMicroseconds(10);  
      }
    }
  }
}
