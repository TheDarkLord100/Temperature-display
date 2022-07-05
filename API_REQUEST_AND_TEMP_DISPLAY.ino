#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "Your SSID";
const char* password = "Your wifi password";
String url = "https://api.openweathermap.org/data/2.5/weather?lat={}&lon={}&appid={api key}";


#define wled 2
#define led1 16
#define led2 5
#define led3 4
#define led4 0
#define led5 14
#define led6 12


void setup() {
  // put your setup code here, to run once:
  pinMode(wled, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);

  digitalWrite(wled, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);

  Serial.begin(115200);
  Serial.print("Wifi connecting");

  WiFi.begin(ssid, password);

  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED )
  {
    delay(200);
    Serial.println(".");
  }
}


void loop() {
  // put your main code here, to run repeatedly:

  if (WiFi.status() == WL_CONNECTED)
    digitalWrite(wled, HIGH);
  delay(200);
  digitalWrite(wled, LOW);
  delay(200);

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  https.begin(client, url);
  int httpCode = https.GET();

  if (httpCode > 0)
  {
    String Payload = https.getString();
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, Payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    JsonObject main = doc["main"];
    float main_temp = main["temp"]; // 302.98
    Serial.println("Temp");
    Serial.println(main_temp);
    int T = main_temp - 273;
    Serial.println(T);
    //digitalWrite(cled, HIGH);
    delay(1000);
    //digitalWrite(cled, LOW);
    if (T % 2 == 1) {
      Serial.println("1");
      digitalWrite(led6, HIGH);
    }
    if (T % 4 > 1) {
      digitalWrite(led5, HIGH);
      Serial.println("2");
    }
    if (T % 8 > 3) {
      digitalWrite(led4, HIGH);
      Serial.println("4");
    }
    if (T % 16 > 7) {
      digitalWrite(led3, HIGH);
      Serial.println("8");
    }
    if (T % 32 > 15) {
      digitalWrite(led2, HIGH);
      Serial.println("16");
    }
    if (T % 64 > 31) {
      Serial.println("32");
      digitalWrite(led1, HIGH);
    }
    //  digitalWrite(led1, HIGH);
    delay(4000);
    delay(4000);
    delay(2000);
    Serial.println("TURN OFF");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    delay(20000);
    delay(20000);
    delay(20000);
  }
  else
    Serial.println("ERROR");
  https.end();
}
