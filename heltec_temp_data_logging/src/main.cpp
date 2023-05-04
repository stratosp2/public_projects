/* The script reads temperature and humidity from DHT11 sensor and shows these data on the monitor. Then, it sends these data to google drive sheets.
All linraries are stored manually in lib folder pulled from https://github.com/HelTecAutomation/Heltec_ESP32 .*/




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
const unsigned long HOUR = 3600*SECOND;
unsigned long dt = 0.5*HOUR;

String SSID   = "WLAN-311316";
String PSW    = "2857576042500438";
String GAS_ID = "AKfycbxUA-iFyShhg8ORREHtRwJY3jWFqtpLwfCxj5u_L8ysTNN8HFssBa_zSQnCJSAAP6sN"; // Google ID sheet.

WiFiClientSecure client;



// Routine for connecting to wifi
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



// Activates the led and screen, display message that all start.
void message()
{
    pinMode(LED,OUTPUT);
	digitalWrite(LED,HIGH);

	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

	delay(300);
	Heltec.display->clear();
    //Heltec.display -> drawString(0, 10, "Hello world, finally.");
	Heltec.display -> drawString(25, 25, "Starting all routines.");
  	Heltec.display -> display();
	
	delay(2000);
	//esp_deep_sleep_start(); // For deepsleep. Maybe for battery usage.

} 


// Displaying data on the screen.
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
	WIFISetUp();
    delay(300);
	Heltec.display->clear();
   	HT.begin();

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

        //testdrawstyles(payload);
      }
    else {
      Heltec.display -> drawString(20,20,"Error on HTTP request");
    }
	http.end();
}

void loop()
{   
	displayReadingsOnOled();
  	spreadsheet_comm();
	delay(dt);
}



 


