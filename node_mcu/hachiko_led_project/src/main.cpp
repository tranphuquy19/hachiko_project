#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DEBUG true

// <<<<<<<<<<<<<<<<<User config>>>>>>>>>>>>>>>>>
const char *ssid = "Quy";
const char *password = "11231123";
const char *mcuEndpoint = "http://192.168.137.1:3000/c81700d4-39df-11ea-a137-2e728ce88125";
const char *nodeID = "45db3f56-a5d7-4412-a122-31bed77269a3";

const uint8_t trig = 4; //D2
const uint8_t echo = 5; //D1

const uint8_t lamp = 12; //D6
// <<<<<<<<<<<<<<<<<User config>>>>>>>>>>>>>>>>>

struct GPIO
{
  bool pinD7 = 0; // GPIO13
  bool pinD5 = 0; // GPIO14
  bool pinD0 = 0; // GPIO16
};

typedef struct GPIO states;
const char epsilon = 5;
int currentValue = 0;
int defaultDistance = 0, newDistance = 0, count = 0;

int getDistance()
{
  int duration;
  int distance;

  digitalWrite(trig, LOW);

  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = int(duration / 2 / 29.412);
  if (DEBUG)
  {
    Serial.print(distance);
    Serial.println(" cm");
  }
  return distance;
}

void setup()
{
  Serial.begin(9600);

  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(lamp, OUTPUT);

  pinMode(D7, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D0, OUTPUT);

  digitalWrite(D7, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D0, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2500);
    Serial.print("Connecting...\n");
  }

  defaultDistance = getDistance();
  newDistance = 0;
  count = 0;
}

void sendData(String distance)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    http.begin(mcuEndpoint);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"A0Value\":";
    jsonPayload += String(analogRead(A0));
    jsonPayload += ",\"distance\":";
    jsonPayload += String(distance);
    jsonPayload += ",\"nodeID\":\"";
    jsonPayload += nodeID;
    jsonPayload += "\"";
    jsonPayload += "}";

    http.POST(jsonPayload);
    String resPayload = http.getString();

    if (DEBUG)
    {
      Serial.println(resPayload);
    }
    http.end();
  }
}

void loop()
{
  currentValue = getDistance();

  int delta = defaultDistance - currentValue;
  if (delta > epsilon)
  {
    if (currentValue != newDistance)
    {
      sendData(String(currentValue));
      newDistance = currentValue;
      count = 1;
    }
    else
    {
      if (currentValue == defaultDistance)
      {
        newDistance = 0;
        count = 0;
      }
      else
      {
        if (count == 10)
        {
          defaultDistance = newDistance;
          count = 1;
        }
        else
          count++;
      }
    }
  }

end:
  Serial.println("newDistance - " + String(newDistance));
  delay(1000);
}