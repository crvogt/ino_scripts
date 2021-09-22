#include <SPI.h>
#include <RH_RF95.h>
#include <SparkFun_ADXL345.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

/*****SD Card Record Vars******/
bool is_recording = false;
// Record time in ms
long int record_time = 2000;
long int current_time = 0;
long int start_time = 0;

/**********ADXL Vars***********/
// For SPI communication
ADXL345 adxl = ADXL345(5);

/***********LoRa Vars**********/
bool create_radio = true;
bool init_flag = true;
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blink on receipt
#define LED 13

void setup() 
{
  /********LoRa Setup*********/
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio initialized");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  /********ADXL Setup********/
  adxl.powerOn();
  // Set g range, {2, 4, 8, 16}
  adxl.setRangeSetting(16);
  // Set for 4-wire SPI 
  adxl.setSpiBit(0);  
  ////////////////////
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 
  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);
}

void loop(){
  // LoRa Section, wait for record signal
  while(!is_recording){
      if(rf95.available()){
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
    
      if (rf95.recv(buf, &len)){
        digitalWrite(LED, HIGH);
        RH_RF95::printBuffer("Received: ", buf, len);
        if(atoi((char*)buf) == 1){
          Serial.println("Recording!");
          is_recording = true;
          uint8_t data[] = "Now recording...";
          // Send a reply
          rf95.send(data, sizeof(data));
          rf95.waitPacketSent();
          Serial.println("Sent a reply");
          digitalWrite(LED, LOW);
          start_time = millis();
        }
      }
    }
  }  
  
  if(is_recording && ((millis() - start_time) < record_time)){
    int x, y, z;
    // Reading the accel data
    adxl.readAccel(&x, &y, &z);
    Serial.println(z);
  }
//  current_time = millis();
//  else if(rf95.available())
//  {
//    // Should be a message for us now   
//    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//    uint8_t len = sizeof(buf);
//    
//    if (rf95.recv(buf, &len))
//    {
//      digitalWrite(LED, HIGH);
//      RH_RF95::printBuffer("Received: ", buf, len);
//      if(atoi((char*)buf) == 1){
//        Serial.println("Recording!");
//        is_recording = true;
//        uint8_t data[] = "Now recording...";
//        // Send a reply
//        rf95.send(data, sizeof(data));
//        rf95.waitPacketSent();
//        Serial.println("Sent a reply");
//        digitalWrite(LED, LOW);
//      }
//      else{
//        uint8_t data[] = "Not recording...";
//        // Send a reply
//        rf95.send(data, sizeof(data));
//        rf95.waitPacketSent();
//        Serial.println("Sent a reply");
//        digitalWrite(LED, LOW);
//      }
////      Serial.println("Got: ");
////      Serial.println(atoi((char*)buf));
////      Serial.println(atoi((char*)buf) == 1);
////       Serial.print("RSSI: ");
////      Serial.println(rf95.lastRssi(), DEC);
//     
//    }
//    else
//    {
//      Serial.println("Receive failed");
//    }
//  }
}
