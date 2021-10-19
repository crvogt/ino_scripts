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

/***Analog Pressure Sensor***/
int analogPressurePin = A0;

bool digital_press = true;

void setup() 
{
  // Begin Serial
  Serial.begin(9600);
  
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
  // Read pressure from each device
  float pressure_hPa = mpr.readPressure();
  int analog_pressure = analogRead(analogPressurePin);

  // Begin transmitting
  char radiopacket[16];

  snprintf(radiopacket, 13, "%d,%d", (int)pressure_hPa, (int)analog_pressure);
  
  Serial.println(radiopacket);
  rf95.send((uint8_t *)radiopacket, 16);
  rf95.waitPacketSent();
}
