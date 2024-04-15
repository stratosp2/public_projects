#include "DHT.h"
#define Type DHT11
int sensePin=2;
DHT HT(sensePin,Type);
float humidity;
float tempC;
int setTime = 1000;
int dt =5000;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 HT.begin();
 delay(setTime);
}

void loop() {
  // put your main code here, to run repeatedly:
humidity = HT.readHumidity();
tempC = HT.readTemperature();

Serial.print("Humidity: ");
Serial.print(humidity);
Serial.print(" %");
Serial.print(" Temperature: ");
Serial.print(tempC);
Serial.println(" C: ");
delay(dt);

}
