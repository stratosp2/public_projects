#include <Arduino.h>


const unsigned long SLEEP_SECOND = 1000000;
const unsigned long SLEEP_HOUR = 3600*SLEEP_SECOND;
unsigned long sleep_time = 20*SLEEP_SECOND;

void deepsleep();

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  deepsleep();

}


void deepsleep(){
  esp_sleep_enable_timer_wakeup(sleep_time);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Hello world");
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Going to deep sleep");
  delay(1000);
  esp_deep_sleep_start();

}

void loop() {
  /* put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Hello world");
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  */
}

