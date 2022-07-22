// Library required for servo function
#include <Servo.h>
// Libraries required for BNO055
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// Controller variables
float integral = 0.0;
float prev_err = 0.0;
float prev_t = -1.0;
float dt = 0.0;
float err = 0.0;

float Kp = 1.0, Kd = 0.0, Ki = 0.0;
float sat = 1.0;

float ctrl_out = 0.0;

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

void setup()
{
  //Initialize serial stream
  Serial.begin(115200);
  // Check which pins to attach servos to
  servo_a.attach(9);
  servo_b.attach(11);

  // Initialize BNO
  if(!bno.begin())
  {
    Serial.print("No BNO055 detected...");
    while(1);
  }
  
  // Allow time for setup
  delay(1000);
}

void loop()
{
  if(rpi_pin)
  {
    // Get the multiplication factor for servo control
    ctrl_out = get_ctrl();
    Serial.print("PID out value: ");
    Serial.println(ctrl_out);

    // Servos take a position value between 0 and 180
    // May be better to check  that a PWM input/lib is better
    servo_a_val = servo_mid_val + ctrl_out; 
    servo_b_val = servo_mid_val + ctrl_out;

    servo_a.write(servo_a_val);
    servo_b.write(servo_b_val);
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
