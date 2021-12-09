#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>

const char* ssid = "ssid";
const char* passwd = "password";

void setup() {
  //Open serial communication and wait for port to open:
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd); 

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

}

void loop() {
  if (Serial.available()) {
    Serial.write(Serial.read());
  }

}
