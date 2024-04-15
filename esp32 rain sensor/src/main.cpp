#include <Arduino.h>


float rain;


void setup() {
  // put your setup code here, to run once:
   pinMode(4,INPUT);
  Serial.begin(115200);
}


void loop(){
    rain = analogRead(4);
    Serial.println(rain);
    delay(2000);
}
