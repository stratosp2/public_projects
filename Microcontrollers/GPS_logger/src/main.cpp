#include "header.h"
#include "functions.h"

/* GLOBAL VARIABLES */ 
BMPdata  baseline, bmp_data, bmp_data_old        = {0}; 
GPSdata  gps_data, gps_data_old, gps_data_old_d  = {0};
TIMER    gps_timer, rst_timer                    = {0};
uint32_t last_battery_check                      = 0;
uint32_t last_reset_check                        = 0;
uint32_t value                                   = 0;
uint8_t  gps_read_count                          = 0; 
uint8_t  tft_state                               = LOW;
uint8_t  sd_state                                = HIGH;  
uint8_t  battery_state                           = 4;   
bool     dist_reset                              = false;
Bat_data batt_volt, bat_data                     = {0};

/* LIBRARY OBJECTS */
TinyGPSPlus     GPS;
SFE_BMP180      BMP;
Adafruit_ST7735 TFT = Adafruit_ST7735(CS_TFT, DC, RST);

void setup()
{  
  /* set pins mode */
  pinMode(BUILTIN_LED, OUTPUT); 
  pinMode(CS_TFT,      OUTPUT);
  pinMode(CS_SD,       OUTPUT);
  pinMode(BATT_PIN,    INPUT);
#if RESET_PROTOTYPE
  pinMode(RST_DST_PIN, INPUT);
  /* attach ISR to reset pin*/
  attachInterrupt(RST_DST_PIN, Ext_INT1_ISR, CHANGE);
#endif

  /* set SPI CS pins to initial state */
  digitalWrite(CS_TFT, LOW);
  digitalWrite(CS_SD,  HIGH);

  /* init Serial und Serial2 */
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  /* init TFT display */
  initialize_TFT();

  /* init SD module (CS needs to be flipped, then reflipped) */
  flip_CS();
  gps_data.dist = initialize_SD();
  flip_CS();

  /* init BMP module */
  initialize_BMP();

  /* init GPS module */
  initialize_GPS();

  /* pause and get baseline data */
  delay(2000);
  baseline = get_baseline();
 
  /* pause, clean screen and print data table */
  delay(3000);
  print_table();

  /* set some variables */
  gps_data_old.dist = 999999;
  dist_reset        = false;
}

void loop()
{
#if IS_BATT_CHECKED
  /* check and update battery icon */
  if (millis() - last_battery_check > BATT_CHECK_RATE)
  {
    last_battery_check = millis();
    update_battery_state(analogRead(BATT_PIN), battery_state, bat_data);
  }
  float voltage = bat_data.battery_voltage; 
  //Serial.print("batt_voltage is: ");
  //Serial.println();
#endif

#if RESET_PROTOTYPE
  if (dist_reset)
  {
    /* start reset countdown, reset distance */
    timeout(rst_timer, reset_distance, gps_data.dist);

    /* make sure that timer stops if button is released within timeout */
    if (millis() - last_reset_check > 500)
    {
      if (!digitalRead(RST_DST_PIN))
      {
        dist_reset = false;
      }
    }
  }
  else
  {
    /* reset timer variables */
    rst_timer.is_timer_running      = false;
    rst_timer.is_timeout_act_active = false;
  }
#endif

  /* perform GPS measurement (intrinsic module sampling ~1Hz) */
  bool is_gps_updated = read_GPS(gps_data);

  if (is_gps_updated)
  {
    /* every time gps is succesfully updated: */
    /* 0. reset signal lost countdown, print red icon */
    gps_timer.is_timer_running = false;

    if (gps_timer.is_timeout_act_active)
    {
      color_GPS_icon(value = ST77XX_YELLOW);
      gps_timer.is_timeout_act_active = false;
    }

    /* 1. attach barometric data */
    read_BMP(baseline, bmp_data); 

    /* 2. update distance traveled */
    gps_read_count++;

    if (gps_read_count == DIST_MESH)
    {
      gps_read_count = 0; 
      update_DIST(gps_data, gps_data_old_d);

      /* save distance in SD in array form */
      flip_CS();
      write_distance(gps_data.dist);
      flip_CS();
    }

    /* 3. save data on SD card (CS needs to be flipped) */
    flip_CS();
    update_SD(gps_data, bmp_data, bat_data);
    flip_CS();

    /* 4. print data on TFT */
    print_data(gps_data, bmp_data, gps_data_old, bmp_data_old);
  }
  else
  {
    /* start signal lost countdown, print white icon if no signal */
    timeout(gps_timer, color_GPS_icon, value = ST77XX_WHITE);
  }
}