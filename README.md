# A02YYUWSensor
A helper class for reading sensor data from [A02YYUW sensor modules](https://wiki.dfrobot.com/_A02YYUW_Waterproof_Ultrasonic_Sensor_SKU_SEN0311) from microcontrollers like an Arduino or ESP.

[A02YYUW Sensor](docs/A02YYUW_sensor.jpg)

# Usage example

```cpp
#include <A02YYUWSensor.h>
#include <SoftwareSerial.h>

#define PIN_TX 6 // TX doesn't matter
#define PIN_RX 5

SoftwareSerial softSerial(PIN_RX, PIN_TX); // TX doesn't matter
A02YYUWSensor sensor;

void setup() {
    softSerial.begin(9600);
    sensor.begin();
}

void loop() {
    while (softSerial.available()) {
        char c = softSerial.read();
        sensor.read(c);

        if (sensor.haveVal()) {
            Serial.print((unsigned int)sensor.getVal());
            Serial.println("mm");
            sensor.resetHaveVal();
        }
        if (sensor.getError() != 0) {
            if (sensor.getError() == A02YYUW_ERRORCHECKSUM) Serial.println(F("ERR CHKSUM"));
            if (sensor.getError() == A02YYUW_ERRORLOWLIMIT) Serial.println(F("ERR TOO LOW"));
            if (sensor.getError() == A02YYUW_ERRORHIGHLIMIT) Serial.println(F("ERR TOO HIGH"));
            sensor.resetError();
        }
    }
}
```
