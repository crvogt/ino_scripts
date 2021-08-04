#include <SD.h>
const int chipSelect = 4;

int inPin = 3;
int outPin = 2;

int stateLED = LOW;
int reading;
int previous = HIGH;

long time = 0;
long debounce = 200;

void setup(){
  Serial.begin(9600);
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
}

void loop(){
  reading = digitalRead(inPin);

  time = millis();

  digitalWrite(outPin, stateLED);

  previous = reading;

  Serial.begin(9600);
  pinMode(10, OUTPUT);

  if(!SD.begin(chipSelect)){
    //
  }
  String dataString = "";

  // read three sensors and append to the string
  for(int analogPin = 0; analogPin < 2; analogPin++){
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if(analogPin < 1){
      dataString += ",";
    }
  }

  // Open the file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available write to it
  if(dataFile){
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too
    Serial.println(dataString);
  }

  // If the file isn't open, error
  else{
    Serial.println("error opening datalog.txt");
  }
  delay(250);
}



//#include "SD.h"
//#include <Wire.h>
//
//// A simple data logger for the Arduino analog pins
//#define LOG_INTERVAL  100 // mills between entries
//
//// The analog pins that connect to the sensors
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
//  pinMode(greenLEDpin, OUTPUT);
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
