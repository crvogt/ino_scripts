#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>
#include "Adafruit_MPRLS.h"

// LoRa
#define RFM95_CS 6
#define RFM95_RST 2
#define RFM95_INT 3
#define RF95_FREQ 915.0

// Indicator
#define LED_PIN 7

// MPRLS
#define RESET_PIN  -1
#define EOC_PIN    -1

/*******LoRa******/
RH_RF95 rf95(RFM95_CS, RFM95_INT);

/*****MPRLS******/
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

int record_flag = 0;

void setup() 
{
  // Begin Serial
  while (!Serial);
  Serial.begin(9600);
  delay(100);
  
  // Set the indicator LED
  pinMode(LED_PIN, OUTPUT);     

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

  // Set up MPRLS
  if(!mpr.begin()){
    Serial.println("Failed to start MPRLS sensor...");
  }
  else{
    Serial.println("Started MPRLS sensor...");
  }
}

void loop()
{
  float pressure_hPa = mpr.readPressure();

  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
  
    if (rf95.recv(buf, &len))
    {
      char *recv_array = (char *)buf;
      if(recv_array[0] == 'r'){
        record_flag = true;
        digitalWrite(LED_PIN, HIGH);
      }
      else if(recv_array[0] == 's'){
        record_flag = false;
        digitalWrite(LED_PIN, LOW);
      }
    }
  }

  if(record_flag){
    // Send record message
    if(Serial.available()){
      Serial.write('r');
    }
    // Begin transmitting
    char radiopacket[11];
    Serial.println((long int)pressure_hPa);
    snprintf(radiopacket, 8, "%d", (long int)pressure_hPa);
    Serial.println(radiopacket);
    rf95.send((uint8_t *)radiopacket, 11);
    rf95.waitPacketSent();
  }  
}
