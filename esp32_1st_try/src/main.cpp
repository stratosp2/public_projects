#include "heltec.h"
#include "WiFi.h"
#include "images.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define Type DHT11
int sensePin=34; // #8 from heade j3 side.
DHT HT(sensePin,Type);
float humidity;
float tempC;


void message(){
    pinMode(LED,OUTPUT);
	digitalWrite(LED,HIGH);

	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

	delay(300);
	Heltec.display->clear();
    //Heltec.display -> drawString(0, 10, "Hello world, finally.");
	Heltec.display -> drawString(40, 25, "I am here.");
    Heltec.display -> display();
	//delay(5000);
	//Heltec.display->clear();
    //Heltec.display -> drawString(40, 25, "Going to sleep.");
	//Heltec.display -> display();
	delay(2000);
	//esp_deep_sleep_start();

}

/*
void WIFISetUp(void)
{
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.disconnect(true);
	delay(1000);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoConnect(true);
	WiFi.begin("WLAN-311316","2857576042500438");
	delay(100);

	byte count = 0;
	while(WiFi.status() != WL_CONNECTED && count < 10)
	{
		count ++;
		delay(500);
		Heltec.display -> drawString(0, 0, "Connecting...");
		Heltec.display -> display();
	}

	Heltec.display -> clear();
	if(WiFi.status() == WL_CONNECTED)
	{
		Heltec.display -> drawString(0, 0, "Connected.");
		Heltec.display -> display();
		//delay(500);
	}
	else
	{
		Heltec.display -> clear();
		Heltec.display -> drawString(0, 0, "Failed to connect.");
		Heltec.display -> display();
		//while(1);
	}
	Heltec.display -> drawString(0, 10, "WiFi Setup done.");
	Heltec.display -> display();
	delay(500);
}

void WIFIScan(void)
{
	Heltec.display -> drawString(0, 20, "Starting scan.");
	Heltec.display -> display();

	int n = WiFi.scanNetworks();
	Heltec.display -> drawString(0, 30, "Scan done.");
	Heltec.display -> display();
	delay(500);
	Heltec.display -> clear();

	if (n == 0)
	{
		Heltec.display -> clear();
		Heltec.display -> drawString(0, 0, "No network found");
		Heltec.display -> display();
		while(1);
	}
	else
	{
    Serial.print(n);
		Heltec.display -> drawString(0, 0, (String)n);
		Heltec.display -> drawString(14, 0, "Networks found:");
		Heltec.display -> display();
		delay(500);

		for (int i = 0; i < n; ++i) {
		// Print SSID and RSSI for each network found
			Heltec.display -> drawString(0, (i+1)*9,(String)(i + 1));
			Heltec.display -> drawString(6, (i+1)*9, ":");
			Heltec.display -> drawString(12,(i+1)*9, (String)(WiFi.SSID(i)));
			Heltec.display -> drawString(90,(i+1)*9, " (");
			Heltec.display -> drawString(98,(i+1)*9, (String)(WiFi.RSSI(i)));
			Heltec.display -> drawString(114,(i+1)*9, ")");
			//display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
			delay(10);
		}
	}

	Heltec.display -> display();
	delay(800);
	Heltec.display -> clear();

}

*/


void displayReadingsOnOled() {
	humidity = HT.readHumidity();
	tempC = HT.readTemperature();
	//Serial.print(humidity);
	//Serial.println(tempC);
   
  String temperatureDisplay ="Temperature: " + (String)tempC +  "°C";
  String humidityDisplay = "Humidity: " + (String)humidity + "%";
 
  // Clear the OLED screen
  Heltec.display->clear();
  // Prepare to display temperature
  Heltec.display->drawString(0, 0, temperatureDisplay);
  // Prepare to display humidity
  Heltec.display->drawString(0, 12, humidityDisplay);
  // Display the readings
  Heltec.display->display();
}

void setup()
{
    message();
    delay(2000);
    delay(300);
	Heltec.display->clear();
	//WIFISetUp();
   	HT.begin();

}


void loop()
{   
	displayReadingsOnOled();
	delay(2000);
 /* 
 WIFIScan();
 WIFISetUp();
 if(WiFi.status() == WL_CONNECTED)
	{
        Heltec.display -> clear();
		Heltec.display -> drawString(0, 0, "Connected.");
		Heltec.display -> display();
		//delay(500);
	}
    else {
        Heltec.display -> clear();
		Heltec.display -> drawString(0, 0, "Failed to connect.");
		Heltec.display -> display();
    }
 delay(2000);
 */
}


