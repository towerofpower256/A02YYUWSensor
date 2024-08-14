#include <Arduino.h>
#include <A02YYUWSensor.h>
#include <SoftwareSerial.h>

#define PIN_TX 6 // PIN doesn't matter
#define PIN_RX 5
#define PIN_SENSORPWR 2 // GPIO PIN that's powering the sensor
#define PIN_DETAIL 3 // GPIO PIN to tell the sensor to take more sensitive readings

SoftwareSerial softSerial(PIN_RX, PIN_TX);
A02YYUWSensor sensor;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting"));
  
  softSerial.begin(9600);
  sensor.begin();
  
  // Power the module via the GPIO pin
  pinMode(PIN_SENSORPWR, OUTPUT);
  digitalWrite(PIN_SENSORPWR, HIGH);
}

void loop() {
  while (softSerial.available()) {
    uint8_t c = softSerial.read();
    
    /*
    if (c < 16) Serial.print("0");
    Serial.print(c, HEX);
    Serial.print(" ");
    */

    // Read the next char
    sensor.read(c);

    if (sensor.haveVal()) {
      Serial.print(sensor.getVal(), DEC);
      Serial.println("mm");
      sensor.resetHaveVal();
    }
    if (sensor.getError() != 0) {
      if (sensor.getError() == A02YYUW_ERRORCHECKSUM) Serial.println(F("ERR CHKSUM FAIL"));
      if (sensor.getError() == A02YYUW_ERRORLOWLIMIT) Serial.println(F("ERR TOO LOW"));
      if (sensor.getError() == A02YYUW_ERRORHIGHLIMIT) Serial.println(F("ERR TOO HIGH"));
      sensor.resetError();
    }
  }
}