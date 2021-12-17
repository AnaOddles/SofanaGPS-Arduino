#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WifiLocation.h>
#include <ArduinoJson.h> 

//Global Variables 

//Store the lat and lon coorindates
String lat; 
String lon; 

//Network Credentials
const char* ssid = "ssid";
const char* passwd = "pass";

//The serial connection to the espmodule wifi board 
//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

//Function for configuring ESP8266 upon bootup
void setup() {
  //Open serial communication and wait for port to open:
  Serial.begin(9600);
  nodemcu.begin(9600);
  
  while (!Serial) continue;
  
  //Call function to connect to Wifi
  connectToWifi();
}

//Function that runs when ESP8266 is running - loops
void loop() {
  //If Serial Communication from Arduino is successful
  if (nodemcu.available()) {
    Serial.println("Wifi Board recieving data\n"); 

  //Setup Json buffer that will store GPS coordinates as JSON
  StaticJsonBuffer<1000> jsonBuffer; 
  //Parse the Json object passed from Arduino (GPS coorindates)
  JsonObject& data = jsonBuffer.parseObject(nodemcu); 

  //If error parsing - 
  if (data == JsonObject::invalid()){
    Serial.println("Invalid Json Object");
    jsonBuffer.clear(); 
    return; 
    }

   //Extract the coorindates from Json Object
   lat = String(data["lat"]); 
   lon = String(data["lon"]); 
   
  //Display coodinates for debugging
  displayCoorindates(data);

   delay(5000);
  }
  else{
    Serial.println("Waiting for communication from GPS\n"); 
    return;
    }
}

//Function to connect system to Wifi network
void connectToWifi(){
  
  //Start Wifi Connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd); 
  Serial.print("Connecting");

  //Loop until we get a connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  //Display connection information for debugging
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

}

//Function to display the GPS coordinates for debugging purposes
void displayCoorindates(JsonObject& data){
  //Sucessfully parsed Json Object 
   Serial.println("______________________________________");
   Serial.println("Json Object Recieved\n"); 
   Serial.print("Recieved Latitude: "); 
   Serial.println(lat); 
   Serial.print("Recieved Longitude:  ");
   Serial.println(lon); 
   Serial.println("______________________________________");
  }
 

  
