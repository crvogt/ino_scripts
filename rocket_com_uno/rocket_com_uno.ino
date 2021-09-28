/*
 * Uno to communicate with ground station and save
 * pressure and temperature data as well as send 
 * to ground uno.
 * Triggers accelerometer recording on sibling uno
 */
#include <SD.h>
#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345(5);
long int lstart_time = 0;
// Record for two minutes
long int record_time = 120000;

void setup(){
  Serial.begin(9600);
  
  adxl.powerOn();
  adxl.setRangeSetting(16);
  adxl.setSpiBit(0);
  adxl.setRate(3200);

  Serial.println(adxl.getRate());
  delay(20000);
}

void loop(){
  if(Serial.available()){
    if(Serial.read() == 'r'){
      Serial.write('w');
      while(lstart_time - millis() < record_time){
        int x, y, z;
        adxl.readAccel(&x, &y, &z);
        Serial.println(z);
        delayMicroseconds(10);  
      }
    }
  }
}
