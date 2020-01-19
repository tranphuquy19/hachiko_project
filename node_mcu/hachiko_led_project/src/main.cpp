#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "Quy";
const char *password = "11231123";
const char *mcuEndpoint = "http://192.168.137.1:3000/c81700d4-39df-11ea-a137-2e728ce88125";

const char lamp = 5;

void setup()
{
  Serial.begin(9600);

  pinMode(lamp, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting...\n");
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(mcuEndpoint);
    int httpStatusCode = http.GET();

    if (httpStatusCode > 0)
    {
      String payload = http.getString();
      int result = payload.toInt();
      Serial.print(result);
      if (0 == result)
      {
        Serial.print('-');
        digitalWrite(lamp, HIGH);
        delay(2000);
        digitalWrite(lamp, LOW);
      }
      Serial.println(payload);
    }
    http.end();
  }
  delay(6000);
}