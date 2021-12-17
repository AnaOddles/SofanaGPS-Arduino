#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h> 

//Global Variables

//Serial Pins for GPS Module
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

//Storing the gps coordinates
String longitude; 
String latitude; 

// The TinyGPS++ object
TinyGPSPlus gps;

//The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//The serial connection to the espmodule wifi board 
SoftwareSerial espSerial(5,6); 

//Function for configuring Arduino upon bootup
void setup()
{
  //Start the Serial Monitor for debugging 
  Serial.begin(115200);
  //Start the Serial connection for Wifi module
  espSerial.begin(9600);
  //Start the Serial connection for GPS module
  ss.begin(GPSBaud);

  delay(1000); 
  Serial.println("Program Started...\n");
}

//Function that runs when arduino is running - loops
void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      //Call function to send data to Wifi Module
      sendData();
      
  //If we are not receiving any encoding from GPS Module
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

//Function to send data to Wifi Module
void sendData()
{
  //Setup json buffer that will store GPS coordinates as JSON
  StaticJsonBuffer<1000> jsonBuffer; 
  JsonObject& data = jsonBuffer.createObject(); 

  //Grab long and lat coordinates 
  grabCoordinates(); 

  //Assign data to JSON objet 
  data["lat"] = latitude; 
  data["lon"] = longitude; 

  //Send data to wifi module 
  data.printTo(espSerial); 

  //Clear the buffer for next coorindates 
  jsonBuffer.clear(); 

  //Wait 5 seconds
  delay(5000); 
  
  Serial.println();
}

//Function that used to grab coorindates from the GPS Module
void grabCoordinates(){
  //If a valid gps location is grabbed
  if (gps.location.isValid())
  {
    //Grab a string of lat and lon for debugging
    //str = "Location: " + String(gps.location.lat(), 6) + " ," + String(gps.location.lng(), 6); 
    //espSerial.println(str); 

    //Print to the Arduino Serial Monitor for debugging
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);

    //Grab the coords 
    longitude = String(gps.location.lng(), 6); 
    latitude = String(gps.location.lat(), 6); 
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }
}
