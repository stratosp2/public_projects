#include <Wire.h>
#include <SPI.h>
#include <heltec.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

Adafruit_BME280 bme;

float tempC;
float humidity;
float pressure;
float altitude;

//int sda=34;
//int scl=35;

void setup(){
    Serial.begin(115200);
    Heltec.begin(true, false, true);
    //Wire.begin(sda, scl); // SDA=34, SCL=35
    bme.begin(0x76);
}

void loop(){
    tempC = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100.0;
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %");
    Serial.print(" Temperature: ");
    Serial.print(tempC);
    Serial.print(" C: ");
    Serial.print(" Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa: ");
    delay(2000);
}

