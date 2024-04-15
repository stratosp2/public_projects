/* The script reads temperature and humidity from DHT11 sensor and shows these data on the monitor. Then, it sends these data to google drive sheets.
All linraries are stored manually in lib folder pulled from https://github.com/HelTecAutomation/Heltec_ESP32 .

This is not a battery friendly code. I use it to operate only inside the house, showing the temperature on the screen and send data every hour
to a new google sheet only for inside. 

*/

#include "heltec.h" // for the microcontroller.
#include "WiFi.h"
#include "images.h" // Imported locally in lib folder. Stored in /Users/stratos/Documents/PlatformIO/Projects/images.h
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <HTTPClient.h>

#define Type DHT11
int sensePin=34; // #8 from header j3 side. See https://resource.heltec.cn/download/WiFi_Kit_32_V3/HTIT-WiFi%20kit32_V3(Rev1.1).pdf
DHT HT(sensePin,Type);
float humidity;
float tempC;

const unsigned long SECOND = 1000;
const unsigned long SLEEP_SECOND = 1000000;
const unsigned long HOUR = 3600*SECOND;
const unsigned long SLEEP_HOUR = 3600*SLEEP_SECOND;
unsigned long sleep_time = 10*SLEEP_SECOND;
unsigned long dt = 1*HOUR;

String SSID   = "WLAN-311316";
String PSW    = "2857576042500438";
String GAS_ID = "AKfycbyYM2OZwEhYzCFNuDq9kZhRpH0vnA_ZOADaA5SmiAaEY7nmsyMn0MhksvgD0c-9kSHnTw"; // Google deployment ID sheet.

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
	while(WiFi.status() != WL_CONNECTED && count < 20)
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
	//Heltec.display -> drawString(0, 10, "WiFi Setup done.");
	//Heltec.display -> display();
	//delay(500);
}



// Activates the led and screen, display message that all start.


// Displaying data on the screen.
void displayReadingsOnOled() {
	humidity = HT.readHumidity();
	tempC = HT.readTemperature();
	//Serial.print(humidity);
	//Serial.println(tempC);
   
  String temperatureDisplay ="Temp: " + (String)tempC +  "°C";
  String humidityDisplay = "Hum: " + (String)humidity + "%";
 
  // Clear the OLED screen
  Heltec.display->clear();
  // Prepare to display temperature
  Heltec.display->drawString(0, 0, temperatureDisplay);
  // Prepare to display humidity
  Heltec.display->drawString(0, 20, humidityDisplay);
  // Display the readings
  Heltec.display->display();
  delay(1000);

}

// Sending data to google sheet.
void spreadsheet_comm() {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GAS_ID+"/exec?temperature=" + (String)tempC + "&humidity=" + (String)humidity;
//   Serial.print(url);
	Serial.print("Making a request");
	http.begin(url.c_str()); //Specify the URL and certificate
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
		Heltec.display -> drawString(40,40,"Data sent");
       // Heltec.display -> drawString(20,20,(String)httpCode);
        //Heltec.display -> drawString(20,40,payload);
		Heltec.display -> display();
		  delay(1000);
        //testdrawstyles(payload);
      }
    else {
      Heltec.display -> drawString(20,20,"Error on HTTP request");
    }
	http.end();
}


void setup()
{
	pinMode(LED,OUTPUT);
	//digitalWrite(LED,HIGH);
	//esp_sleep_enable_timer_wakeup(sleep_time);
   	HT.begin();
	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
	//delay(300);
	Heltec.display->setFont(ArialMT_Plain_16); // Change the font	
	Heltec.display -> drawString(40,40,"Sending data");
	Heltec.display->display();
  // Prepare to display humidity
	displayReadingsOnOled();
	//delay(500);
	Heltec.display->display();
	//delay(800);
	//digitalWrite(LED,LOW);
	//esp_deep_sleep_start();


}


void loop()
{   
	displayReadingsOnOled();
	WIFISetUp();
	digitalWrite(LED,HIGH);
	spreadsheet_comm();
	digitalWrite(LED,LOW);
	displayReadingsOnOled();
	delay(dt);

}



 


