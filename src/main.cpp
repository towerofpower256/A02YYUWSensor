#include <Arduino.h>
#include <A02YYUWSensor.h>
#include <SoftwareSerial.h>

#define PIN_TX 6
#define PIN_RX 5
#define PIN_SENSORPWR 2
#define PIN_DETAIL 3

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
  // put your main code here, to run repeatedly:
  while (softSerial.available()) {
    uint8_t c = softSerial.read();
    
    /*
    //Serial.print("0x");
    if (c < 16) Serial.print("0");
    Serial.print(c, HEX);
    Serial.print(" ");
    */

    sensor.read(c);

    if (sensor.haveVal()) {
      Serial.print((unsigned int)sensor.getVal());
      Serial.println("mm");
      sensor.resetHaveVal();
    }
    if (sensor.getError() != 0) {
      if (sensor.getError() == A02YYUW_ERRORCHECKSUM) Serial.println(F("ERR CHKSUM"));
      if (sensor.getError() == A02YYUW_ERRORLOWLIMIT) Serial.println(F("ERR LOWVAL"));
      sensor.resetError();
    }
  }
}