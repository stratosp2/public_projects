#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>
#include "WiFi.h"
#include <Arduino.h>

Adafruit_BME280 bme;


/*Comment out all delay routines for battery efficiency!*/

float tempC;
float humidity;
float pressure;
float altitude;
float rain;
int wind;

/* The relevant formulas assume two 100kOhm resistors connected*/
float bat_reading;
double bat_voltage;
int bat_pin = 35;
int wind_pin= 15; 


const unsigned long SECOND = 1000;
const unsigned long SLEEP_SECOND = 1000000; // sleep in microseconds
const unsigned long HOUR = 3600*SECOND;
const unsigned long SLEEP_HOUR = 3600*SLEEP_SECOND;
unsigned long sleep_time = 1*SLEEP_HOUR;
unsigned long dt = 1*SECOND;

//debug or not? If true set the send data to false to go into the loop
#define debug  false

// When in loop set to false
#define send_data true

String SSID   = "Home 2.4G";
String PSW    = "your wifi password";
String GAS_ID = "google sheet  ID"; // Deployment ID. Can be found via google sheet.
WiFiClientSecure client;




// Routine for connecting to wifi
void WIFISetUp(void)
{
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.disconnect(true);
	delay(500);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoConnect(true);
	WiFi.begin("Home 2.4G","your wifi password");
	//delay(100);

	byte count = 0;
	while(WiFi.status() != WL_CONNECTED && count < 15)
	{
		count ++;
		delay(500);
		Serial.println("Connecting...");
	}

	if(WiFi.status() == WL_CONNECTED)
	{
		Serial.println("Connected.");
	}
	else
	{
	
		Serial.println("Failed to connect.");

		}
}


// Sending data to google sheet.
void spreadsheet_comm() {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GAS_ID+"/exec?temperature=" + (String)tempC + "&humidity=" + (String)humidity + "&pressure=" + (String)pressure + "&rain=" + (String)rain + "&wind=" + (String)wind + "&voltage=" + (String)bat_voltage;
//   Serial.print(url);
	Serial.println("Making a request");
	http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
		Serial.println("Data sent");

      }
    else {
      Serial.println("Error on HTTP request");
    }
	http.end();
}


void setup(){
	  pinMode(LED_BUILTIN, OUTPUT);
    pinMode(4,INPUT);
    pinMode(bat_pin,INPUT);
    Serial.begin(115200);
    bme.begin(0x76);
    rain =analogRead(4);
    bat_reading = analogRead(bat_pin);
    wind = analogRead(wind_pin);
    Serial.print(wind);
    bat_voltage = (4.20*bat_reading)/2312.00; // assumes 2 100kOhm resistors connected, Vout=Vin*R2/(R1+R2). Here Vin=bat_voltage and Vout the one which goes to the pin.
	  #if send_data
      digitalWrite(LED_BUILTIN, HIGH);
      WIFISetUp();
      //delay(500);
      esp_sleep_enable_timer_wakeup(sleep_time);
      tempC = bme.readTemperature();
      humidity = bme.readHumidity();
      pressure = bme.readPressure()/100;
      spreadsheet_comm();
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      esp_deep_sleep_start();
    #endif

}

//optimized for deepsleep. It does not go in the loop

void loop(){
  #if debug
    bat_reading = analogRead(bat_pin);
    bat_voltage = (4.2*bat_reading)/2312; // assumes 2 100kOhm resistors connected, Vout=Vin*R2/(R1+R2). Here Vin=bat_voltage and Vout the one which goes to the pin.
    tempC = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100;
    wind = analogRead(wind_pin);
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(" %");
    Serial.print(" Temperature: ");
    Serial.print(tempC);
    Serial.print(" C: ");
    Serial.print(" Pressure: ");
    Serial.print(pressure);
    Serial.print(F(" hPa: "));
    Serial.print(F(" Battery voltage: "));
    Serial.print(bat_voltage);
    Serial.print(F(" , "));
    Serial.print(bat_reading);
    Serial.print(F(" , "));
    Serial.print(F("Wind:"));
    Serial.println(wind);
    delay(dt);
	#endif 
}

