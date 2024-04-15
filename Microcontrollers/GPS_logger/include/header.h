/* LIBRARIES */
#include <Arduino.h>
#include "BluetoothSerial.h"
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>
#include <SD.h>
#include <TinyGPSPlus.h>
#include <SFE_BMP180.h>
#include <Wire.h>

/* CONSTANTS/MACROS */
// define altitude sample size
#define BASELINE_SIZE     5
// set lower limit for gps refresh rate
#define GPS_RATE_MIN      200
// set gps read distance error
#define GPS_ERROR         2.5
// set timeout signal out/distance reset
#define TIMEOUT           3000          
// set distance mesh
#define DIST_MESH         5
// set Serial2 pins for GT-U7
#define RX_PIN            16
#define TX_PIN            17
// set pins for TFT
#define DC                32
#define RST               4
#define CS_TFT            5
// set pins for SD
#define CS_SD             15
// set this to true to activate battery icon
#define IS_BATT_CHECKED   true
// set pin for battery
#define BATT_PIN          25
// set battery check rate
#define BATT_CHECK_RATE   5
// set battery min value
#define BATT_MIN          1000  
// reset distance pin
#define RST_DST_PIN       33      
// set this to true to activate reset prototype
#define RESET_PROTOTYPE   true

/* STRUCTURES */
// structure for gps data
typedef struct
{
  double   lat, lng, alt, spd;
  uint32_t dist;
  uint16_t year;
  uint8_t  mon, day;
  uint8_t  hour, min, sec;
} GPSdata;

//structure for bmp data
typedef struct 
{
  double tmp, prs, alt;
} BMPdata;

//structure for timer
typedef struct
{
  bool     is_timer_running;
  bool     is_timeout_act_active;
  uint32_t start_time;
  uint32_t curr_time;
} TIMER;

//structure for battery data
typedef struct 
{
  float battery_voltage;
} Bat_data;