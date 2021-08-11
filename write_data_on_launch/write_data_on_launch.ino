#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

const int chipSelect = 4;

int inPin = 3;
int outPin = 2;

int stateLED = LOW;
int reading;
int previous = HIGH;
bool bstart_write = false;
File data_file;

long time = 0;
long write_start_time = 0; 
bool bstart_write_time = true;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
float x = -100000, y = -100000, z = -100000;
sensors_event_t linear_accel_data;

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
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
}

void loop(){

  // Get current time in ms
  time = millis();

  // Get accel from sensor
  bno.getEvent(&linear_accel_data, Adafruit_BNO055::VECTOR_LINEARACCEL);

  if(linear_accel_data.acceleration.z > 2){
    bstart_write = true;
  }
  else if(!bstart_write){
    Serial.println("Waiting to start");
    delay(100);
  }
  if(bstart_write){
    if(bstart_write_time){
      write_start_time = time;
      bstart_write_time = false;
    }
    if((time - write_start_time) < 1000){
      Serial.print(linear_accel_data.acceleration.x);
      Serial.print(",");
      Serial.print(linear_accel_data.acceleration.y);
      Serial.print(",");
      Serial.print(linear_accel_data.acceleration.z);
      Serial.print("\n");
      
      data_file.println(time);
      
      // delayMicroseconds(100);
      delay(100);
    }
    else if((time - write_start_time) > 1000){
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
