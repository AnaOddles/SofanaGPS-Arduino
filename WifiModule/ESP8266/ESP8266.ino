#include <SoftwareSerial.h>
void setup() {
  //Open serial communication and wait for port to open: 
  Serial.begin(115200);

}

void loop() {
  if(Serial.available()){
    Serial.write(Serial.read());
    }

}
