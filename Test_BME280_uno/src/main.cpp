#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

/*
Adafruit_BME280 bme;

float tempC;
float humidity;
float pressure;
float altitude;
*/

float rain;


void setup(){
    pinMode(A3,INPUT);
    Serial.begin(115200);
    //bme.begin(0x76);
}

void loop(){
    rain = analogRead(A3);
    Serial.println(rain);
    delay(2000);
}

