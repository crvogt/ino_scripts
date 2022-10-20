// Library required for servo function
#include <Servo.h>
// Libraries required for BNO055
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
// Libraries required for MPRLS
#include <Wire.h>
#include "Adafruit_MPRLS.h"
// Data smoothing
#include <movingAvg.h>
#include <math.h>

// Controller variables
bool start_sig = false;
float integral = 0.0;
float prev_err = 0.0;
float prev_t = -1.0;
float dt = 0.0;
float err = 0.0;
// PID parameters
float Kp = 5.0, Kd = 0.0, Ki = 0.0;
float sat = 80.0;
// Control value initialization
float ctrl_out = 0.0;
// Velocity goal in y axis
double y_goal = 0.0;

// Servo variables
Servo servo_a, servo_b;
float servo_a_val = 0.0, servo_b_val = 0.0;
float servo_mid_val = 90.0;

// BNO055 variables
// Set delay between fresh samples
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10;
// Check I2C device address (default is 0x29 or 0x28)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// MPRLS variables
#define RESET_PIN -1
#define EOC_PIN -1
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
// Get a moving average of pressure changes
int mApnts = 20;
movingAvg mAdp(mApnts);

// Note the starting pressure
bool init_flag = true;
float threshold_pressure = 2.0;
float start_pressure = -1000.0; 
float active_pressure = -1000.0;
float scaling_factor = 10.0;
float current_pressure = -1000.0;
float previous_pressure = -1000.0;

void setup()
{
  //Initialize serial stream
  Serial.begin(115200);
  
  // Check which pins to attach servos to
  servo_a.attach(9);
  servo_b.attach(10);

  // Initialize BNO
  if(!bno.begin())
  {
    Serial.print("No BNO055 detected...");
    while(1);
  }
  else{
    Serial.println("BNO055 detected...");
  }

  // Initialize MPRLS
  if(!mpr.begin()){
    Serial.println("Failed to communicate with MPRLS sensor");
    while(1) {
      delay(10);
    }
  }
  else{ 
    Serial.println("Found MPRLS sensor");
  }

  // Initialize movingAvg
  mAdp.begin();
}

void loop()
{
  /*
   * Desc
   * Start a moving average for our pressure gradient.
   * We first need to populate the mAdp structure
   * Theory - on ascent, the pressure gradient is negative
   * on descent, the sign swaps, at which point we start 
   * a timer and activate the fins and controller
   * 
   * TODO
   * 
   * Test gradient switch
   * 
   * Test PID output
   * 
   */
  // Add readings to mAdp
  if(mAdp.getCount() == mApnts){
    if(init_flag){
      // Scaling was added to avoid some loss of resolution
      start_pressure = mAdp.getAvg()/scaling_factor; //mpr.readPressure();
      Serial.println(mAdp.getCount());
      Serial.println("Start pressure: ");
      Serial.println(start_pressure);
      init_flag = false;
    }
    previous_pressure = current_pressure;
    current_pressure = mpr.readPressure() * scaling_factor;
    if(previous_pressure > -900)
      mAdp.reading(int(roundf(current_pressure - previous_pressure)));
  }  
  else{
    //Serial.println("Adding reading...");
    Serial.println(mpr.readPressure());
    mAdp.reading(int(roundf(mpr.readPressure() * scaling_factor)));
  }

  // Wait for positive pressure gradient and that we've ascended enough
  //if(!init_flag) //bypass
  if((mAdp.getAvg() > 0) && (abs(current_pressure - start_pressure) > threshold_pressure))
  {
    start_sig = true;
    // Delay while bomblet is deployed
    Serial.println("Starting fin control...");
    delay(4000);
  }
 
  if(start_sig) 
  {
    // Make sure we don't trip delay() again
    while(1){
      // Get the multiplication factor for servo control
      ctrl_out = get_ctrl();
      //Serial.println(ctrl_out);
      // Servos take a position value between 0 and 180
      // May be better to check  that a PWM input/lib is better
      servo_a_val = servo_mid_val + ctrl_out; 
      servo_b_val = servo_mid_val + ctrl_out;
  
      servo_a.write(servo_a_val);
      servo_b.write(servo_b_val);
    }
  }
}

float get_ctrl()
{
  sensors_event_t angVelocityData;
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  double x = 0.0, y = 0.0, z = 0.0, t = 0.0;
  x = angVelocityData.gyro.x;
  y = angVelocityData.gyro.y;
  z = angVelocityData.gyro.z;

  // We only have one axis to deal with, y
  err = y_goal - y;
  
return regulate(err, millis());
}


float regulate(float _err, float _t)
{
  float u = 0.0;
  float derr_dt = 0.0;
  dt = _t - prev_t;

  if(prev_t > 0.0 && dt > 0.0)
  {
    derr_dt = (_err - prev_err) / dt;
    integral += 0.5 * (_err + prev_err) * dt;
  }

  u = Kp * _err + Kd * derr_dt + Ki * integral;

  prev_err = _err;
  prev_t = _t;

  if(u > sat)
  {
    u = sat;
    integral = 0.0;
  }
  
  return u;
}
