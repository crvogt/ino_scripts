#include <SD.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BNO055.h>

//#define BNO055_SAMPLERATE_DELAY_MS (1)


const int chipSelect = 4;

int inPin = 3;
int outPin = 2;

//int counter = 0;
float fcounter = 0.0;
long lstart_time = 0;
long lcurrent_time = 0;
bool bstart_time = true;
int analogPin = A3;

float test_val = 0;

File data_file;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
sensors_event_t event;

void setup() {
  Serial.begin(115200);

  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  pinMode(10, OUTPUT);

  // Check SD is available
  if(!SD.begin(chipSelect)){ 
    Serial.println("Struggling to find SD card");
    while(1){}
  }
  else{
    // Open the file
    // create a new file
    char filename[] = "LOGGER00.CSV";
    for (uint8_t i = 0; i < 100; i++) {
      filename[6] = i/10 + '0';
      filename[7] = i%10 + '0';
      if (! SD.exists(filename)) {
        // only open a new file if it doesn't exist
         data_file = SD.open(filename, FILE_WRITE); 
         Serial.print("Opening new file ");
         Serial.print(filename);
         Serial.println();
         break;  // leave the loop!
        }
      }
  
    Wire.begin();
  }
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  else{
    /* Use external crystal for better accuracy */
    bno.setExtCrystalUse(true);
  }
  
  Serial.println("Beginning...");
  delay(1000);

}

void loop() {

  if(bstart_time){
    lstart_time = millis();
    bstart_time = false;
  }

  if((lcurrent_time - lstart_time) < 1000){
//    counter++;
    fcounter += 1;
    lcurrent_time = millis();
//    bno.getEvent(&event);
    test_val = analogRead(analogPin);
    data_file.print(fcounter);
    data_file.print("\n");  
  }
  else{
    Serial.print("Counter value... ");
    Serial.print(fcounter);
    Serial.print(" in time(ms): ");
    Serial.print(lcurrent_time - lstart_time);
    Serial.println();
    data_file.close();
    while(1){}
  }
  

}
