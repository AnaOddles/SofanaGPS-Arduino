#include <ESP8266WiFi.h> 
#include <WifiLocation.h>
//Import the wifi library

const char* googleApiKey = "API_KEY";
const char* ssid = "WIFI_NAME";
const char* passwd = "WIFI_PASSWORD";
WifiLocation location(googleApiKey);

// Set time via NTP, as required for x.509 validation
void setClock () {
    configTime (0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print ("Waiting for NTP time sync: ");
    time_t now = time (nullptr);
    while (now < 8 * 3600 * 2) {
        delay (500);
        Serial.print (".");
        now = time (nullptr);
    }
    struct tm timeinfo;
    gmtime_r (&now, &timeinfo);
    Serial.print ("\n");
    Serial.print ("Current time: ");
    Serial.print (asctime (&timeinfo));
}

void setup() {
 Serial.begin(115200);
 Serial.println();
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, passwd);

 Serial.print("Connecting");
 while(WiFi.status() != WL_CONNECTED)
 {
  delay(500);
  Serial.println(".");
  setClock();
 }
 Serial.println(); 
 Serial.print("Connected, IP address: ");
 Serial.println(WiFi.localIP());
 WiFi.printDiag(Serial);

 location_t loc = location.getGeoFromWiFi();
 Serial.println("Location request data");
 Serial.println(location.getSurroundingWiFiJson());
 Serial.println("Latitude: " + String(loc.lat, 7));
 Serial.println("Longitude: " + String(loc.lon, 7));
 Serial.println("Accuracy: " + String(loc.accuracy));
 Serial.println ("Result: " + location.wlStatusStr (location.getStatus ()));
}

void loop() {
  // put your main code here, to run repeatedly:

}
