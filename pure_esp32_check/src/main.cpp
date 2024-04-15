#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>
#include "WiFi.h"

Adafruit_BME280 bme;


/*Comment out all delay routines for battery efficiency!*/

float tempC;
float humidity;
float pressure;
float altitude;
float rain;


const unsigned long SECOND = 1000;
const unsigned long SLEEP_SECOND = 1000000;
const unsigned long HOUR = 3600*SECOND;
const unsigned long SLEEP_HOUR = 3600*SLEEP_SECOND;
unsigned long sleep_time = 1*SLEEP_HOUR;
unsigned long dt = 1*HOUR;

String SSID   = "WLAN-311316";
String PSW    = "2857576042500438";
String GAS_ID = "AKfycbxqKnCtNS1ZKv43bwDMMpCnWvw3ceCSqV6lmdfqmaZ_g08iNSyZ_tSkzrEvRR0V_L8t"; // Google ID sheet.

WiFiClientSecure client;

// Routine for connecting to wifi
void WIFISetUp(void)
{
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.disconnect(true);
	delay(500);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoConnect(true);
	WiFi.begin("WLAN-311316","2857576042500438");
	//delay(100);

	byte count = 0;
	while(WiFi.status() != WL_CONNECTED && count < 10)
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

	//Serial.println("WiFi Setup done.");
	//delay(100);
}


// Sending data to google sheet.
void spreadsheet_comm() {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GAS_ID+"/exec?temperature=" + (String)tempC + "&humidity=" + (String)humidity + "&pressure=" + (String)pressure + "&rain=" + (String)rain;
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
    Serial.begin(115200);
    bme.begin(0x76);
    rain =analogRead(4);
	digitalWrite(LED_BUILTIN, HIGH);
    WIFISetUp();
    //delay(500);
	esp_sleep_enable_timer_wakeup(sleep_time);
	tempC = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100;
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %");
    Serial.print(" Temperature: ");
    Serial.print(tempC);
    Serial.print(" C: ");
    Serial.print(" Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa: ");
    Serial.print(" Rain: ");
    Serial.print(rain);
    Serial.println(" : ");
    spreadsheet_comm();
	///delay(3000);
   delay(500);
	digitalWrite(LED_BUILTIN, LOW);
  	esp_deep_sleep_start();

}

//optimized for deepsleep

void loop(){
   /* tempC = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure()/100;
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %");
    Serial.print(" Temperature: ");
    Serial.print(tempC);
    Serial.print(" C: ");
    Serial.print(" Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa: ");

    spreadsheet_comm();
    delay(dt);
	*/
}

