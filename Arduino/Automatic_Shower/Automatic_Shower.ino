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

uint32_t time;
bool enableHeater = false;
uint32_t t0 = millis();
int pos = 0 // between 0 and 180


void setup() {
  Serial.begin(9600);

  while (!Serial)
    delay(10);

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

  Serial.println("Time (sec),Temp (*C),Humidity (%)");
}


void loop() {
  float temp = sht31.readTemperature();
  float hum = sht31.readHumidity();

  if (! isnan(temp)) {  // check if 'is not a number'
    // Serial.print("Temp *C = "); Serial.print(temp); Serial.print("\t\t");
  } else { 
    // Serial.println("Failed to read temperature");
  }
  
  if (! isnan(hum)) {  // check if 'is not a number'
    // Serial.print("Hum. % = "); Serial.print(hum);
  } else { 
    // Serial.println("Failed to read humidity");
  }

  time = millis() - t0;
  // Serial.print("\t\tTime = "); Serial.println(time * 1e-3);

  Serial.print(time * 1e-3); Serial.print(",");
  Serial.print(temp); Serial.print(",");
  Serial.println(hum);

  delay(1000);

}
