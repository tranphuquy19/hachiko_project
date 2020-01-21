#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "Quy";
const char *password = "11231123";
const char *mcuEndpoint = "http://192.168.137.1:3000/c81700d4-39df-11ea-a137-2e728ce88125";

const char trig = 4;
const char echo = 5;

const char lamp = 12;
const char epsilon = 10;

int preValue = 0, currentValue = 0;
int defaultDistance = 0, newDistance = 0, count = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(lamp, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2500);
    Serial.print("Connecting...\n");
  }
}

int getDistance()
{
  unsigned long duration;
  int distance;

  digitalWrite(trig, LOW);

  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = int(duration / 2 / 29.412);

  // Serial.print(distance);
  // Serial.println(" cm");

  return distance;
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    http.begin(mcuEndpoint);
    http.addHeader("Content-Type", "application/json");

    char *testJSON = "{\"sensorData\":12}";
    int httpCode = http.POST(testJSON);
    String payload = http.getString();

    Serial.println(payload);
    http.end();
  }
  delay(2000);
}

// void loop()
// {
//   preValue = currentValue;
//   currentValue = getDistance();

//   if (preValue == 0 || defaultDistance == 0 || newDistance == 0)
//   {
//     preValue = currentValue;
//     defaultDistance = currentValue;
//     newDistance = currentValue;
//     return;
//   }

//   int delta = currentValue - preValue;
//   delta = abs(delta);
//   if (delta >= epsilon && currentValue != defaultDistance)
//   {
//     Serial.print("Detected! | ");
//     Serial.print(delta);
//     Serial.print("-");
//     Serial.print(preValue);
//     Serial.print("-");
//     Serial.println(currentValue);
//   }

//   if (currentValue != defaultDistance)
//   {
//     if (count == 10)
//     {
//       defaultDistance = newDistance;
//       count = 0;
//       Serial.print("Changed default distance to: ");
//       Serial.println(defaultDistance);
//     }

//     if (newDistance != currentValue)
//     {
//       newDistance = currentValue;
//       count = 0;
//     }
//     else
//       count++;
//   }
//   delay(500);

// }