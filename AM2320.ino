// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <Wire.h>
#include <AM2320.h>
AM2320 th;
// WiFi network info.
char ssid[] = "UPCFFB3577";
char wifiPassword[] = "wmkcsyApwt5x";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "a20a6ed0-e53c-11e6-a446-0d180dc59d42";
char password[] = "a4644b018f8f2c854ade28f434d8885ddcf39733";
char clientID[] = "f5ce1900-5bf5-11e8-9a81-53c2c26c817c";

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();

  //Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 10000) {
    switch (th.Read()) {
      case 2:
        Serial.println(" CRC failed");
        break;
      case 1:
        Serial.println(" Sensor offline");
        break;
      case 0:
        Serial.print(" Humidity = ");
        Serial.print(th.Humidity);
        Serial.println("%");
        Serial.print(" Temperature = ");
        Serial.print(th.cTemp);
        Serial.println("*C");
        Serial.println();
        break;
    }
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    //Cayenne.virtualWrite(0, lastMillis);
    //Some examples of other functions you can use to send data.
    Cayenne.celsiusWrite(1, th.cTemp);
    Cayenne.virtualWrite(2, th.Humidity);
    Cayenne.virtualWrite(0, th.Humidity,"Vlhkost","%");
    //Cayenne.luxWrite(2, 700);
    //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
  }
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
