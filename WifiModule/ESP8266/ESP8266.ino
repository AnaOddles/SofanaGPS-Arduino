#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

//Global Variables

//Store the lat and lon coorindates
String lat;
String lon;

//Network Credentials
const char* ssid = "ssid";
const char* passwd = "psswrd";

//The serial connection to the espmodule wifi board
//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

//Object that will connect to wifi
ESP8266WiFiMulti WiFiMulti;

//Constant for server IP
//#define SERVER_IP "10.211.55.4:1234"

//Function for configuring ESP8266 upon bootup
void setup() {
  //Open serial communication and wait for port to open:
  Serial.begin(9600);
  nodemcu.begin(9600);

  while (!Serial) continue;

  //Setup time
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  //Call function to connect to Wifi
  connectToWifi();
}

//Function that runs when ESP8266 is running - loops
void loop() {
  //If Serial Communication from Arduino is successful
  if (nodemcu.available()) {
    Serial.println("______________________________________");
    Serial.println("Wifi Board recieving data\n");

    //Setup Json buffer that will store GPS coordinates as JSON
    StaticJsonBuffer<1000> jsonBuffer;
    //Parse the Json object passed from Arduino (GPS coorindates)
    JsonObject& data = jsonBuffer.parseObject(nodemcu);

    //If error parsing - clear buffer and return out of iteration
    if (data == JsonObject::invalid()) {
      Serial.println("Invalid Json Object");
      jsonBuffer.clear();
      return;
    }

    //Extract the coorindates from Json Object
    lat = String(data["lat"]);
    lon = String(data["lon"]);

    //Display coodinates for debugging
    displayCoorindates(data);

    //Sent HTTP POST
    sendHTTPPOST(data);
    Serial.println("______________________________________");

  }
  else {
    Serial.println("Waiting for communication from GPS\n");
    return;
  }
  delay(10000);
}

//Function to connect system to Wifi network
void connectToWifi() {

  //Start Wifi Connection
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, passwd);
  Serial.print("Connecting");

  //Loop until we get a connection
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  //Display connection information for debugging
  Serial.println();
  Serial.print("Connected to WIFI:");
  //Serial.println(WiFi.);
}

//Function to display the GPS coordinates for debugging purposes
void displayCoorindates(JsonObject& data) {
  //Sucessfully parsed Json Object
  Serial.println("Json Object Recieved\n");
  Serial.print("Recieved Latitude: ");
  Serial.println(lat);
  Serial.print("Recieved Longitude:  ");
  Serial.println(lon);
}

//Function to send HTTP Post to SofanaGPSAPI
void sendHTTPPOST(JsonObject& data) {

  //Declare String var for SofanaGPSAPI
  String sofanaGPSAPI_URL = "https://sofanagpsapi.azurewebsites.net/api/locations";
  Serial.println("TESTING POST\n");

  //Make sure Wifi Module is connected to Wifi
  if (WiFiMulti.run() == WL_CONNECTED) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    //Declare an object of class HTTPClient
    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    //Specify the request destination
    if (https.begin(*client, sofanaGPSAPI_URL)) {  // HTTPS

      //Add HTTP Request Headers
      https.addHeader("Content-Type", "application/json");
      //http.addHeader("auth-key", "My_authenication_key");

      String jsonString;
      data.printTo(jsonString);
      Serial.println(jsonString);

      //Send the request
      int httpCode = https.POST(jsonString);

      //Check the returning code
      if (httpCode > 0) {
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          //Get the request reponse payload
          String payload = https.getString();
          //Print the response payload
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      //Close the connection
      https.end();
    }
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

}
