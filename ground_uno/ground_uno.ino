#include <SD.h>
#include <Wire.h>

#include <RH_RF95.h>

// LoRa
#define RFM95_CS 6
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 915.0

// LED
#define INDICATOR_PIN 9

const int toggle_read_pin = 7;

/********SD Card*****/
const int chip_select = 10;
File data_file;

/*******LoRa******/
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup(){
  Serial.begin(9600);
  pinMode(toggle_read_pin, INPUT);
  pinMode(INDICATOR_PIN, OUTPUT);

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
  // Set the LoRa
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if(!rf95.init()) {
    Serial.println("LoRa radio init failed");
  }
  else{
    Serial.println("LoRa radio initialized...");
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
  }
  Serial.print("Frequency set to: "); Serial.println(RF95_FREQ);  
}

void loop(){
  if(digitalRead(toggle_read_pin)){
    digitalWrite(INDICATOR_PIN, HIGH);
    
    char radiopacket[11];
    snprintf(radiopacket,8,"%c", 'r');
    rf95.send((uint8_t *)radiopacket, 11);
    rf95.waitPacketSent();

    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    // Wait for record flag
    if(rf95.waitAvailableTimeout(1000)){
      if(rf95.recv(buf, &len)){
        Serial.println((char*)buf);
        data_file.println((char *)buf);
      }
    }
    // Begin recording incoming data

  }
  else{
    digitalWrite(INDICATOR_PIN, LOW);
  }
}
