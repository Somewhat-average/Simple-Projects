/*************************************************** 
  This is an example for the SHT31-D Humidity & Temp Sensor

  Designed specifically to work with the SHT31-D sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/
 
#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Servo myservo;

uint32_t time, time_prev, t0;
float hum, temp;
bool enableHeater = false;
int pos = 0; // between 0 and 180
int state = 0;
float target_hum = 55.0;

/* low pass Infinite Impulse Response (IIR) filter*/
double dt;
double tau = 10; /* Time constant in seconds */
double alpha;
double y;

void setup() {
  Serial.begin(9600);

  while (!Serial)
    delay(10);

  Serial.println("Serial Initialized");

  if (! sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");

  myservo.attach(9);
  myservo.write(pos);

  t0 = millis();
  Serial.println("Time (sec),Temp (*C),Humidity (%), Filtered Humidity (%)");

  delay(100);
  y = sht31.readHumidity();
}


void loop() {
  temp = sht31.readTemperature();
  hum = sht31.readHumidity();
  time_prev = time;
  time = millis() - t0;
  dt = (time - time_prev) * 1e-3;
  alpha = dt / tau;

  /* low pass filter implementation */
  y += alpha * (hum - y);

  Serial.print(time * 1e-3, 3); Serial.print(",");
  Serial.print(temp, 6); Serial.print(",");
  // Serial.print(hum, 6); Serial.print(",");
  Serial.println(y, 6);

  if (state == 0 && y > target_hum*1.1) {
    state++;
  }
  if (y < target_hum && state == 1) {
    pos = 180;
    state = 0;
    myservo.write(pos);
  }

  delay(1000);
}
