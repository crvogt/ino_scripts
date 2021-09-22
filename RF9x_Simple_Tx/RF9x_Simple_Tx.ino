#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(57600);
  delay(100);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed...");
    while (1);
  }
  Serial.println("LoRa radio initialized...");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Frequency set to: "); Serial.println(RF95_FREQ);
}

//int16_t packetnum = 0;  // packet counter, we increment per xmission
uint8_t packetnum = 0;
void loop()
{
//  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  char radiopacket[33];
  itoa(packetnum, radiopacket, 10);
  packetnum++;
//  Serial.print("Sending "); Serial.println(radiopacket);
  
//  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)5, 20);
  packetnum++;

//  rf95.waitPacketSent();

//  delayMicroseconds(10);
}
