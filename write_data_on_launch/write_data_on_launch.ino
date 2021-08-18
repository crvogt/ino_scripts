#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

const int chipSelect = 4;

int inPin = 3;
int outPin = 2;

int stateLED = LOW;
int reading;
int previous = HIGH;
bool bstart_write = false;
File data_file;

long ltime = 0;
long write_start_time = 0; 
bool bstart_write_time = true;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
//Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);


void setup(){
  Serial.begin(115200);
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  pinMode(10, OUTPUT);

  // Check SD is available
  if(!SD.begin(chipSelect)){ 
    Serial.println("Struggling to find SD card");
  }
  else{
    // Open the file
    Serial.println("Opening datalog.txt");
    data_file = SD.open("datalog.txt", FILE_WRITE);
  }

  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("No ADXL345 detected ... Check wiring");
    while (1);
  }
  else{
    accel.setDataRate(ADXL345_DATARATE_3200_HZ);
    accel.setRange(ADXL345_RANGE_2_G);
  }
}

void loop(){

  // Get current time in ms
  ltime = millis();

  // Get accel from sensor
  sensors_event_t event;
  accel.getEvent(&event);

  if(event.acceleration.z > 12){
    bstart_write = true;
  }
  else if(!bstart_write){
    Serial.println("Waiting to start");
//    delay(100);
    //delayMicroseconds(100);
  }
  if(bstart_write){
    if(bstart_write_time){
      write_start_time = ltime;
      bstart_write_time = false;
    }
    if((ltime - write_start_time) < 10000){
    //      Serial.print(event.acceleration.x);
    //      Serial.print(",");
    //      Serial.print(event.acceleration.y);
    //      Serial.print(",");
    //      Serial.print(event.acceleration.z);
    //      Serial.print("\n");

      data_file.print(event.acceleration.x);
      data_file.print(",");
      data_file.print(event.acceleration.y);
      data_file.print(",");
      data_file.print(event.acceleration.z);
      data_file.print("\n");
//      data_file.println(time);
      
      //delayMicroseconds(100);
//      delay(100);
    }
    else if((ltime - write_start_time) > 10000){
      data_file.close();
      Serial.println("File closed...");
      while(1){}
    }
  }
}



//#include "SD.h"
//#include <Wire.h>
//
//#define LOG_INTERVAL  100 // mills between entries
//#define loadcellPin 0           // analog 0
//
//// Use digital pin 10 for the SD cs line
//const int chipSelect = 10;
//
//int collectTimeS = 6; // Collection time in seconds
//
//bool start_rec = false;
//bool start_rec_init = true;
//int test_loop_count = 0;
//uint32_t m;
//
//File logfile;
//
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("Setup");
//}
//
//
//void loop() {  
//
//    // Loop for a while for kicks
//    if(test_loop_count > 100){
//      start_rec = true;
//    }
//    else{
//      test_loop_count++;
//    }
//
//    // Setup the card
//    if(start_rec){
//      if(start_rec_init){
//        Serial.begin(115200);
//        // initialize the SD card
//        Serial.print("Initializing SD card...");
//        pinMode(10, OUTPUT);
//    
//        // See if the card is present and can be initialized
//        if (!SD.begin(chipSelect)) {
//          Serial.println("Card failed, or not present");
//          while(1){}
//        }
//  
//        // create a new file
//        char filename[] = "LOGGER00.CSV";
//        for (uint8_t i = 0; i < 100; i++) {
//          filename[6] = i/10 + '0';
//          filename[7] = i%10 + '0';
//          if (! SD.exists(filename)) {
//            // only open a new file if it doesn't exist
//            logfile = SD.open(filename, FILE_WRITE); 
//            break;  // leave the loop!
//          }
//        }
//  
//        Wire.begin();  
//    
//        logfile.println("millis, voltage");    
//      }  
//    }  
//    start_rec_init = false;
//  
//    // fetch the time
//    if(start_rec){
//      // Running main code
//      m = millis();
//      // delay for the amount of time we want between readings
//      delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
//      logfile.print(m);
//    }
//  
//    int loadcellReading = analogRead(loadcellPin);      
//    
//    // converting that reading to voltage, for 3.3v arduino use 3.3
//    float voltage = (loadcellReading * 5.0) / 1024.0;  
//    if(start_rec){
//      logfile.print(", ");    
//      logfile.print(loadcellReading);
//      logfile.print("\n");
//    }
//    
//    if(start_rec){
//      if (m > (collectTimeS * 1000)){
//      Serial.print("Closing file...\n");
//      logfile.close();
//      while(1);
//      }
//    }
//  }
