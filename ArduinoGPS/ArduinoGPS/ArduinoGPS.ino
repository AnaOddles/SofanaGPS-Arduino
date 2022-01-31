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
String dateTime;

// The TinyGPS++ object
TinyGPSPlus gps;

//The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//The serial connection to the espmodule wifi board
SoftwareSerial espSerial(5, 6);

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
    while (true);
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

  //Grab the date time
  grabDateTime();

  if (!latitude.equals("") && !longitude.equals(""))
  {
    //Assign data to JSON objet
    data["lat"] = latitude;
    data["lon"] = longitude;
    data["dateTime"] = dateTime;
    //Send data to wifi module
    data.printTo(espSerial);

    //Clear the buffer for next coorindates
    jsonBuffer.clear();

    //Wait 10 seconds
    delay(10000);
    Serial.println();

  }
  else
    Serial.println(F("Waiting for GPS to grab coordinates...\n"));
}

//Function that used to grab coorindates from the GPS Module
void grabCoordinates() {
  //If a valid gps location is grabbed
  if (gps.location.isValid())
  {
    //Print to the Arduino Serial Monitor for debugging
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print("\n");
    //Grab the coords
    longitude = String(gps.location.lng(), 6);
    latitude = String(gps.location.lat(), 6);
  }
  else
  {
    Serial.print(F("INVALID COORDINATES\n"));
  }
}

//Function that used to grab date time from the GPS Module
void grabDateTime() {
  dateTime = "";
  if (gps.date.isValid())
  {
    if (gps.date.month() < 10) dateTime.concat(String("0"));
    dateTime.concat(String(gps.date.month()));
    dateTime.concat(String("/"));
    if (gps.date.day() < 10) dateTime.concat(String("0"));
    dateTime.concat(String(gps.date.day()));
    dateTime.concat("/");
    dateTime.concat(String(gps.date.year()));
    dateTime.concat(" ");

    if (gps.time.isValid())
    {
      if (gps.time.hour() < 10) dateTime.concat(String("0"));
      dateTime.concat(String(gps.time.hour()));
      dateTime.concat(String(":"));
      if (gps.time.minute() < 10) dateTime.concat(String("0"));
      dateTime.concat(String(gps.time.minute()));
      dateTime.concat(String(":"));
      if (gps.time.second() < 10) dateTime.concat(String("0"));
      dateTime.concat(String(gps.time.second()));
    }
    else
    {
      dateTime.concat(String("INVALID TIME\n"));
    }
    Serial.print(dateTime);
    Serial.print("\n");
  }
  else
  {
    Serial.print(F("INVALID DATETIME\n"));
  }
}
