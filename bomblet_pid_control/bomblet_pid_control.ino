#include <Wire.h>
#include <Adafruit_BNO055.h>

float kP = 0.0;
float kI = 0.0;
float kD = 0.0;
float sat = 0.0;
float dt = 0.0;
float prev_t = 0.0;
float derr_dt = 0.0;
float prev_err = 0.0;
float integral = 0.0;
float u = 0.0;

void setup()
{
  Serial.begin(9600);
  Serial.println("In setup...");
  delay(100);
}

void loop()
{
  Serial.println("Looping...");
}

float regulate(float _t, float _err)
{
  derr_dt = 0.0;
  dt = _t - prev_t;

  if(prev_t > 0.0 && dt > 0.0)
  {
    derr_dt = (_err - prev_err) / dt;
    integral += 0.5 * (_err + prev_err) * dt;
  }
  
  u = kP * _err + kD * derr_dt + kI * integral;

  prev_err = _err;
  prev_t = _t;

  if(u > sat)
  {
    u = sat;
    integral = 0.0;
  }

  return u;
}
