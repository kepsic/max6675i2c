/*
 * RPi2c - test i2c communication between an Arduino and a Raspberry Pi.
 *
 * Copyright (c) 2013 Carlos Rodrigues <cefrodrigues@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//Ref https://gist.github.com/carlosefr/5849220
//https://github.com/adafruit/MAX6675-library/blob/master/examples/serialthermocouple/serialthermocouple.pde
#include <Wire.h>
#include "max6675.h"


#define SLAVE_ADDRESS 0x04
static const char ledPin = 13;
static const char thermoDO = 3;
static const char thermoCS = 4;
static const char thermoCLK = 5;
static const char vccPin = 2;

short ledValue = 0;
short sensorCurr = 0;
short sensorPrev = -10;


MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


void setup()
{
  Serial.begin(9600);
  pinMode(vccPin, OUTPUT); //MAX6675 vcc
  digitalWrite(vccPin, HIGH); 
  pinMode(ledPin, OUTPUT); // OnBoard led 
  Wire.begin(SLAVE_ADDRESS);
  //i2c
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.println("MAX6675 test");   // wait for MAX chip to stabilize
  delay(500);

}

short get_c_temp() {
   double tmp = thermocouple.readCelsius()*100;
   short temperature = (short) tmp;
   return temperature;
}


void loop() {
    sensorCurr = get_c_temp();
    digitalWrite(ledPin,LOW);
    delay(1000);

}


void receiveEvent(int bytes) {
  int operation = Wire.read();
  
  // Change the LED to HIGH on read...
  if (operation == 0x01) {
     digitalWrite(ledPin,HIGH);
  }

  // Consume any remainder bytes...
  while (Wire.available()) {
    Wire.read();
  }
}


void requestEvent() {
  // Both the Arduino and RPi are little-endian, no conversion needed...
  Wire.write((uint8_t *)&sensorCurr, sizeof(sensorCurr));
  Serial.println(sensorCurr);   // Print value
}
