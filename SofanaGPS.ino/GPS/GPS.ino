#include <ESP8266WiFi.h> 
#include "TinyGPS++.h"
#include <SoftwareSerial.h>

TinyGPSPlus gps;
const char* ssid = "LOPES";
const char* passwd = "";

SoftwareSerial ss(4,5);


void setup() {
Serial.begin(115200);
ss.begin(9600);
Serial.println();
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, passwd);

Serial.print("Connecting");
while(WiFi.status() != WL_CONNECTED)
{
  delay(500);
 
  Serial.println(); 
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);
 }
}

void loop() {
// while (ss.available() > 0)
// {
//   gps.encode(ss.read());
//   if (gps.altitude.isUpdated())
//  {
//     Serial.println("READING GPS");
//   }
//   else 
//     Serial.println("Could not read GPS");
//
//   }
//    
//    Serial.println("Could not read GPS");
  

}
