#include "header.h"
#include "functions.h"

extern TinyGPSPlus     GPS;
extern SFE_BMP180      BMP;
extern Adafruit_ST7735 TFT;
extern uint32_t        last_reset_check;
extern uint32_t        value;
extern uint8_t         circle2_bits[];
extern uint8_t         circle2small_bits[];
extern uint8_t         circle2_large_bits[];
extern uint8_t         altitude2_icon_bits[];
extern uint8_t         temperature_icon_esterno_bits[];
extern uint8_t         temperature_icon_interno_bits[];
extern uint8_t         fox_icon_large_bits[];
extern uint8_t         empty_battery_icon_bits[];
extern uint8_t         half_battery_icon_bits[];
extern uint8_t         full_battery_icon_bits[];
extern uint8_t         sat_icon_full_bits[];
extern uint8_t         sat_icon_three_bits[];
extern uint8_t         sat_icon_two_bits[];
extern uint8_t         sat_icon_one_bits[];
extern uint8_t         sat_icon_zero_bits[];
extern uint8_t         sat_icon_small_bits[];
extern uint8_t         tft_state, sd_state;
extern bool            dist_reset;


/********************************************************************************* 
  @brief: start-up SD module, open/create csv file to log gps and bmp data,
          open/create txt file to store total distance, read total distance from 
          txt file and return it, give visual feedback on TFT
  @param: none 
**********************************************************************************/
uint32_t initialize_SD() 
{
  File     myFile; 
  uint32_t temp_dist = 0;
  
  if(SD.begin(CS_SD))
  {
    Serial.println(F("SD fired up!"));
    digitalWrite(CS_SD,  HIGH);
    digitalWrite(CS_TFT, LOW);
    TFT.drawXBitmap(30, 90, circle2small_bits, 30, 30, ST77XX_GREEN);
    digitalWrite(CS_TFT, HIGH); 
    digitalWrite(CS_SD,  LOW);
   
    if(!SD.exists("/gps_data.csv"))
    {
      Serial.print(F("GPS file does not exist, creating..."));
      myFile = SD.open("/gps_data.csv", FILE_WRITE);
      myFile.print("date,");
      myFile.print("time,");
      myFile.print("latitude,");
      myFile.print("longitude,");
      myFile.print("sat_altitude,");
      myFile.print("bar_altitude,");
      myFile.print("distance,");
      myFile.print("speed,");
      myFile.print("pressure,");
      myFile.print("temperature,");
      myFile.println("voltage");
      Serial.println(F("OK"));
      myFile.close();
    }

    if(!SD.exists("/dist_last.txt"))
    {
      myFile = SD.open("/dist_last.txt", FILE_WRITE);
      myFile.close();
    }
    else
    {
      myFile = SD.open("/dist_last.txt", FILE_READ);
      if (myFile) 
      {
        temp_dist = read_distance(myFile);
        myFile.close();
      }
    }
    digitalWrite(CS_SD,  HIGH);
    digitalWrite(CS_TFT, LOW);
  } 
  else
  {
    Serial.println(F("SD failed!"));
    digitalWrite(CS_SD,  HIGH);
    digitalWrite(CS_TFT, LOW);
    TFT.drawXBitmap(30, 90, circle2small_bits, 30, 30, ST77XX_RED);
    while(true);
  }

  return temp_dist;
}


/********************************************************************************* 
  @brief: start-up BMP180 module, give visual feedback on TFT
  @param: none
**********************************************************************************/
void initialize_BMP()
{
  TFT.setTextColor(ST77XX_WHITE);
  if (BMP.begin())
  {
    Serial.println(F("BMP180: init success"));
    TFT.drawXBitmap(17, 43, circle2_bits, 40, 40, ST77XX_GREEN);
  }
  else
  {
    Serial.println(F("BMP180: init fail"));
    TFT.drawXBitmap(17, 43, circle2_bits, 40, 40, ST77XX_RED);
    while(true); 
  }
}


/********************************************************************************* 
  @brief: tart-up GT-U7 module, establish GPS fix, give visual feedback on TFT
  @param: none
**********************************************************************************/
void initialize_GPS()
{
  uint32_t start_check    = millis();  
  uint16_t color          = ST77XX_WHITE;
  uint32_t start_fix      = 0;
  uint8_t  count          = 0;
  uint8_t  count_temp     = 0;
  uint8_t  battery_state  = 4; 

  while (millis() - start_check < 6000)
  {
    if (Serial2.available() > 0)
    {
      GPS.encode(Serial2.read());
    }

    count_temp = (millis() - start_check) / 1000;

    if (count_temp != count)
    {
      draw_arc(81, 103, 30, ST77XX_YELLOW, 180, 180 + count_temp * 60);
      count = count_temp;
    }
  }

  draw_arc(81, 103, 30, ST77XX_YELLOW, 0, 360);

  if (GPS.charsProcessed() < 10)
  {
    Serial.println(F("GT-U7:  check fail"));
    TFT.drawXBitmap(63, 41, circle2_large_bits, 80, 80, ST77XX_RED);
    while(true);
  }
  else
  {
    Serial.println(F("GT-U7:  check success"));
    TFT.drawXBitmap(63, 41, circle2_large_bits, 80, 80, ST77XX_GREEN);   
    delay(2000);

    TFT.fillScreen(ST77XX_BLACK);
    TFT.drawRoundRect(2, 2, TFT.width() - 4, 30, 10, ST77XX_WHITE);
    TFT.setTextColor(ST77XX_CYAN);
    TFT.setCursor(40, 10);
    TFT.setTextSize(2);                    
    TFT.print(F("GPS FIX"));
    TFT.setTextColor(ST77XX_WHITE);
    TFT.setTextSize(1);
    TFT.setCursor(18, 43);
    TFT.print(F("Waiting for signal..."));

    TFT.drawXBitmap(58, 62, sat_icon_zero_bits, 50, 50, color);

    count      = 0;
    start_fix  = millis();


  

    while (!GPS.sentencesWithFix())
    {
      #if IS_BATT_CHECKED
      Bat_data bat_data = {0};
      update_battery_state(analogRead(BATT_PIN), battery_state, bat_data);
      //Serial.println(analogRead(BATT_PIN)); //For voltage normalization
      Serial.print("bat_volt is: ");
      Serial.println(bat_data.battery_voltage);
      delay(500);
      #endif

      while (Serial2.available() > 0)
      {
        GPS.encode(Serial2.read());
      }

      count = ((millis() - start_fix) / 1000);
      
      if ((count / 3) % 2) color = ST77XX_BLACK;
      else                 color = ST77XX_WHITE;

      switch (count % 3)
      {
        case 0: 
        TFT.drawXBitmap(58, 62, sat_icon_one_bits, 50, 50, color);
        break;

        case 1: 
        TFT.drawXBitmap(58, 62, sat_icon_two_bits, 50, 50, color);
        break;

        case 2: 
        TFT.drawXBitmap(58, 62, sat_icon_three_bits, 50, 50, color);
        break;
      }   
    }
    
    TFT.setTextColor(ST77XX_BLACK);
    TFT.setCursor(18, 43);
    TFT.print(F("Waiting for signal..."));
    TFT.setTextColor(ST77XX_WHITE);
    TFT.setCursor(18, 43);
    TFT.print(F("Fix successful"));
    TFT.drawXBitmap(58, 62, sat_icon_full_bits, 50, 50, ST77XX_CYAN);
  }
}

/********************************************************************************* 
  @brief: collect GPS altitude sample of size BASELINE_SIZE,
          collect baseline BMP data, compute average altitude and set it as 
          baseline dist, give visual feedback on TFT
  @param: none
**********************************************************************************/
BMPdata get_baseline()
{
  char     str[7];
  double   alt_sum    = 0;
  double   alt_avg    = 0;
  uint32_t alt_count  = 0;
  BMPdata  baseline   = {0};

  TFT.fillScreen(ST77XX_BLACK);

  TFT.setTextColor(ST77XX_CYAN);
  TFT.drawRoundRect(2, 2, TFT.width()-4, 30, 10, ST77XX_WHITE);

  TFT.setTextSize(2);
  TFT.setCursor(34, 10);
  TFT.print(F("BASELINE"));

  TFT.setTextColor(ST77XX_WHITE);
  TFT.setTextSize(1);
  TFT.drawRoundRect(10, 45, TFT.width()-20, 20, 1, ST77XX_WHITE);
  TFT.drawXBitmap(20, 76, altitude2_icon_bits, 59, 40, ST77XX_WHITE);
  TFT.setCursor(103, 102);
  TFT.print("meters");
  
  TFT.setTextSize(2);

  while (alt_count < BASELINE_SIZE)
  {
    while (Serial2.available() > 0)
    { 
      if (GPS.encode(Serial2.read()))
      {
        if (GPS.altitude.isValid() && GPS.altitude.isUpdated() && 
           (GPS.altitude.age() > GPS_RATE_MIN)) 
        {
          TFT.setTextColor(ST77XX_BLACK);
          dtostrf(alt_avg,  5,  0, str);
          TFT.setCursor(80, 82);
          TFT.print(str);

          alt_count += 1;
          alt_sum   += GPS.altitude.meters();  
          alt_avg    = alt_sum / alt_count;

          TFT.fillRoundRect(10, 45, (alt_count * (TFT.width()-20)) / BASELINE_SIZE, 
                            20, 1, ST77XX_YELLOW);
          
          if (alt_count < BASELINE_SIZE) TFT.setTextColor(ST77XX_WHITE);
          else                           TFT.setTextColor(ST77XX_CYAN);
          dtostrf(alt_avg,  5,  0, str);
          TFT.setCursor(80, 82);
          TFT.print(str);
        }
      }
    }
  }

  TFT.setTextSize(1);

  /* get baseline data for bmp */
  read_BMP(baseline, baseline);
  baseline.alt = alt_avg;

  return baseline;
}


/********************************************************************************* 
  @brief: initialize TFT and print initial icons
  @param: none
**********************************************************************************/
void initialize_TFT()
{
  TFT.initR(INITR_BLACKTAB);
  TFT.fillScreen(ST77XX_BLACK);
  print_logo();
  TFT.setRotation(1);
  TFT.drawRoundRect(2, 2, TFT.width()-4, 30, 10, ST77XX_WHITE);
  TFT.setTextColor(ST77XX_CYAN);
  TFT.setCursor(8, 10);
  TFT.setTextSize(2);
  TFT.print(F("INIT SENSORS"));
  TFT.setTextSize(1);
  TFT.setTextColor(ST77XX_WHITE);

  TFT.setCursor(39, 102);
  TFT.print(F("SD"));
  TFT.drawXBitmap(30, 90, circle2small_bits, 30, 30, ST77XX_WHITE);

  TFT.setCursor(29, 60);
  TFT.print(F("BMP"));
  TFT.drawXBitmap(17, 43, circle2_bits, 40, 40, ST77XX_WHITE);

  TFT.setCursor(86, 73);
  TFT.setTextSize(2);
  TFT.print(F("GPS"));
  TFT.setTextSize(1);
  TFT.drawXBitmap(63, 41, circle2_large_bits, 80, 80, ST77XX_WHITE);
}


/********************************************************************************* 
  @brief: print start-up logo after bootup
  @param: none
**********************************************************************************/
void print_logo()
{
  TFT.fillScreen(ST77XX_BLACK);
  TFT.setTextColor(ST77XX_WHITE);
  TFT.setTextSize(3);
  TFT.setRotation(1);
  TFT.drawRoundRect(2, 2, TFT.width() - 4, TFT.height() - 4, 10, ST77XX_WHITE);
  TFT.drawXBitmap((TFT.width() - 126) / 2, 1, fox_icon_large_bits, 125, 125, 
                  ST77XX_CYAN);
  delay(3000);
  TFT.setTextSize(1);
  TFT.setRotation(0);
  TFT.fillScreen(ST77XX_BLACK);
}


/********************************************************************************* 
  @brief: save current GPS and BMP data on SD (gps_data.csv)
  @param: gps_data (GPSdata) 
  @param: bmp_data (BMPdata)
**********************************************************************************/
void update_SD(const GPSdata& gps_data, const BMPdata& bmp_data, const Bat_data& bat_data)
{
    File myFile  = SD.open("/gps_data.csv", FILE_APPEND);

    if (myFile)
    { 
      Serial.print(F("txt file opened, writing..."));
      digitalWrite(BUILTIN_LED, HIGH);
      myFile.print(gps_data.year);
      myFile.print("-");
      myFile.print(gps_data.mon);
      myFile.print("-");
      myFile.print(gps_data.day);
      myFile.print(",");
      myFile.print(gps_data.hour);
      myFile.print(":");
      myFile.print(gps_data.min);
      myFile.print(":");
      myFile.print(gps_data.sec);
      myFile.print(",");
      myFile.print(gps_data.lat, 6);
      myFile.print(",");
      myFile.print(gps_data.lng, 6);
      myFile.print(",");
      myFile.print(gps_data.alt, 0);
      myFile.print(",");
      myFile.print(bmp_data.alt, 0);
      myFile.print(",");
      myFile.print(gps_data.dist);
      myFile.print(",");
      myFile.print(gps_data.spd, 1);
      myFile.print(",");
      myFile.print(bmp_data.prs, 2);
      myFile.print(",");
      myFile.print(bmp_data.tmp, 1);
      myFile.print(",");
      myFile.println(bat_data.battery_voltage, 2);  
      myFile.close();
      digitalWrite(BUILTIN_LED, LOW);
      Serial.println(F("done"));
    }
    else 
    {
      Serial.println(F("file did not open"));
    }
}


/********************************************************************************* 
  @brief: print template on TFT with icons and units of measure,
          current data is printed thereon with printData()
  @param: none
**********************************************************************************/
void print_table()
{
  TFT.fillScreen(ST77XX_BLACK);
  TFT.setTextColor(ST77XX_WHITE);
  TFT.setTextSize(1);

  TFT.fillCircle(50, 77, 40, ST77XX_RED);
  TFT.fillCircle(50, 77, 37, ST77XX_BLACK);
  TFT.fillRoundRect(55, 37, 100, 3, 5, ST77XX_RED);
  TFT.setCursor(39, 95);
  TFT.print("km/h");

  TFT.drawXBitmap(11, 6, temperature_icon_interno_bits, 10, 23, ST77XX_ORANGE);
  TFT.drawXBitmap(11, 6, temperature_icon_esterno_bits, 10, 23, ST77XX_YELLOW);
  TFT.setCursor(27, 20);
  TFT.print("degC");

  TFT.setCursor(137, 65);
  TFT.print("km");

  TFT.setCursor(99, 87);
  TFT.print("SAT");
  TFT.setCursor(99, 102);
  TFT.print("BAR");

  color_GPS_icon(value = ST77XX_YELLOW);
}


/********************************************************************************* 
  @brief: current data is processed and printed on TFT:
          - speed
          - total distance
          - state gps signal
          - gps and barometric altitude
          - temperature
  @param: data1     (GPSdata)
  @param: data2     (BMPdata)
  @param: data1_old (GPSdata)
  @param: data2_old (BMPdata)
**********************************************************************************/
void print_data(const GPSdata& data1, const BMPdata& data2, GPSdata& data1_old, 
                     BMPdata& data2_old)
{
  char     str[13];
  double   dtemp, dtemp_old = 0;

  // DISTANCE
  dtemp     = trunc((double)data1.dist / 1000);
  dtemp_old = trunc((double)data1_old.dist / 1000);

  if (dtemp != dtemp_old)
  {
    TFT.setTextSize(2);
    //delete old data
    TFT.setTextColor(ST77XX_BLACK);
    dtostrf(dtemp_old, 4, 0, str);
    TFT.setCursor(94, 46);
    TFT.print(str);
    //print new data
    TFT.setTextColor(ST77XX_WHITE);
    dtostrf(dtemp, 4, 0, str);
    TFT.setCursor(94, 46);
    TFT.print(str);
  }

  dtemp     = trunc(((double)data1.dist - (dtemp * 1000)) / 100); 
  dtemp_old = trunc(((double)data1_old.dist - (dtemp_old * 1000)) / 100); 

  if (dtemp != dtemp_old)
  {
    TFT.setTextSize(1);
    //delete old data
    TFT.setTextColor(ST77XX_BLACK);
    dtostrf(dtemp_old, 1, 0, str);
    TFT.setCursor(143, 53);
    TFT.print(str);
    //print new data
    TFT.setTextColor(ST77XX_WHITE);
    dtostrf(dtemp, 1, 0, str);
    TFT.setCursor(143, 53);
    TFT.print(str);
  }

  dtemp     = trunc(data1.spd);
  dtemp_old = trunc(data1_old.spd);

  // SPEED
  if (dtemp != dtemp_old)
  {
    TFT.setTextSize(3);
    //delete old data
    TFT.setTextColor(ST77XX_BLACK);
    if (dtemp_old < 10)
    {
      dtostrf(dtemp_old, 1, 0, str);
      TFT.setCursor(44, 66);
    }
    else if (dtemp_old < 100) 
    {
      dtostrf(dtemp_old, 2, 0, str);
      TFT.setCursor(35, 66);
    }
    else
    {
      dtostrf(dtemp_old, 3, 0, str);
      TFT.setCursor(25, 66);
    }
    TFT.print(str);
    //print new data
    TFT.setTextColor(ST77XX_WHITE);
    if (dtemp < 10)
    {
      dtostrf(dtemp, 1, 0, str);
      TFT.setCursor(44, 66);
    }
    else if (dtemp < 100) 
    {
      dtostrf(dtemp, 2, 0, str);
      TFT.setCursor(35, 66);
    }
    else
    {
      dtostrf(dtemp, 3, 0, str);
      TFT.setCursor(25, 66);
    }
    TFT.print(str);
  }

  // ALTITUDE SAT
  dtemp     = trunc(data1.alt);
  dtemp_old = trunc(data1_old.alt);

  if (dtemp != dtemp_old)
  {
    TFT.setTextSize(1); 
    //delete old data
    TFT.setTextColor(ST77XX_BLACK);
    dtostrf(dtemp_old, 4, 0, str);
    TFT.setCursor(124, 87);
    TFT.print(str);
    //print new data
    TFT.setTextColor(ST77XX_WHITE);
    dtostrf(dtemp, 4, 0, str);
    TFT.setCursor(124, 87);
    TFT.print(str);
  }

  // ALTITUDE BAR
  dtemp     = trunc(data2.alt);
  dtemp_old = trunc(data2_old.alt);

  if (dtemp != dtemp_old)
  {
    TFT.setTextSize(1); 
    //delete old data
    TFT.setTextColor(ST77XX_BLACK);
    dtostrf(dtemp_old, 4, 0, str);
    TFT.setCursor(124, 102);
    TFT.print(str);
    //print new data
    TFT.setTextColor(ST77XX_WHITE);
    dtostrf(dtemp, 4, 0, str);
    TFT.setCursor(124, 102);
    TFT.print(str);
  }

  // TEMPERATURE
  dtemp     = trunc(data2.tmp * 10) / 10;
  dtemp_old = trunc(data2_old.tmp * 10) / 10; 

  if (dtemp != dtemp_old)
  {
    TFT.setTextSize(1);
    //delete old data
    TFT.setTextColor(ST77XX_BLACK);
    dtostrf(dtemp_old, 5, 1, str);
    if (dtemp_old >= 0) TFT.setCursor(20, 10);
    else                TFT.setCursor(24, 10);
    TFT.print(str); 
    //print new data
    TFT.setTextColor(ST77XX_WHITE);
    dtostrf(dtemp, 5, 1, str);
    if (dtemp >= 0) TFT.setCursor(20, 10);
    else            TFT.setCursor(24, 10);
    TFT.print(str); 
  }

  data1_old = data1;
  data2_old = data2;
}

/********************************************************************************* 
  @brief: color gps icon with input color
  @param: color (uint32_t)
**********************************************************************************/
void color_GPS_icon(uint32_t& color)
{
  TFT.drawXBitmap(TFT.width()-62, 7, sat_icon_small_bits, 23, 23, (uint16_t)color);
}


/********************************************************************************* 
  @brief: set battery state according to read input battery voltage,
          print the corresponding battery icon on TFT
  @param: input      (uint16_t)
  @param: batt_state (uint8_t)
  @param: bat_data (Bat_data) stores a float
**********************************************************************************/
void update_battery_state(const uint16_t& input, uint8_t& batt_state, Bat_data& bat_data)
{
  uint8_t batt_state_old  = batt_state;
  float batt_voltage = 0;
  /* assumes 2 100kOhm resistors connected, Vout=Vin*R2/(R1+R2). 
     Here Vin=bat_voltage and Vout the one which goes to the pin. */
  batt_voltage = (4.20 * input) / 2455.00;
  bat_data.battery_voltage = batt_voltage; 
  //Serial.println(batt_voltage);
  
  if      (batt_voltage > 3.7)                          batt_state = 2;
  else if (batt_voltage <= 3.7 && batt_voltage > 3.4)   batt_state = 1;
  else                                                  batt_state = 0;
  
  //Serial.println(batt_state);

  if (batt_state != batt_state_old)
  {
    switch (batt_state)
    {
      case 0:
      TFT.drawXBitmap(TFT.width()-33, 8, full_battery_icon_bits,  25, 18, 
                      ST77XX_BLACK);
      TFT.drawXBitmap(TFT.width()-33, 8, empty_battery_icon_bits, 25, 18, 
                      ST77XX_RED);
      break;

      case 1:
      TFT.drawXBitmap(TFT.width()-33, 8, full_battery_icon_bits, 25, 18, 
                      ST77XX_BLACK);
      TFT.drawXBitmap(TFT.width()-33, 8, half_battery_icon_bits, 25, 18, 
                      ST77XX_CYAN);
      break;

      case 2:
      TFT.drawXBitmap(TFT.width()-33, 8, full_battery_icon_bits, 25, 18, 
                      ST77XX_BLACK);
      TFT.drawXBitmap(TFT.width()-33, 8, full_battery_icon_bits, 25, 18, 
                      ST77XX_GREEN);
      break;
    }
  }
}


/********************************************************************************* 
  @brief: read GT-U7 sensor, return true if new data is available, false otherwise
  @param: data (GPSdata)
**********************************************************************************/
bool read_GPS(GPSdata& data)
{
  bool is_loc_updated  = false;

  while (Serial2.available() > 0)
  { 
    if (GPS.encode(Serial2.read()))
    {
      if (GPS.location.isValid() && GPS.location.isUpdated() && 
         (GPS.location.age() > GPS_RATE_MIN)) {  
        data.lat = GPS.location.lat();
        data.lng = GPS.location.lng();
        is_loc_updated = true;
      } 
      if (GPS.speed.isValid() && GPS.speed.isUpdated()) {
        data.spd = GPS.speed.kmph();
      } 
      if (GPS.altitude.isValid() && GPS.altitude.isUpdated()) {
        data.alt = GPS.altitude.meters();
      } 
      if (GPS.date.isValid() && GPS.time.isValid() && GPS.time.isUpdated()) {
        data.year = GPS.date.year();
        data.mon  = GPS.date.month();
        data.day  = GPS.date.day();
        data.hour = GPS.time.hour() + 1;
        data.min  = GPS.time.minute();
        data.sec  = GPS.time.second();
      } 
    }
  }
  return is_loc_updated;
}


/********************************************************************************* 
  @brief: read pressure and temperature from BMP180 sensor, 
          compare with baseline and compute relative altitude
  @param: BASELINE (BMPdata)
  @param: data     (BMPdata)
**********************************************************************************/
void read_BMP(const BMPdata& BASELINE, BMPdata& data)
{
  char status  = BMP.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = BMP.getTemperature(data.tmp);
    if (status != 0)
    {
      status = BMP.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = BMP.getPressure(data.prs, data.tmp);
        if (status != 0)
        {
          data.alt = BASELINE.alt + BMP.altitude(data.prs, BASELINE.prs);
        }
        else Serial.println(F("error retrieving pressure measurement"));
      }
      else Serial.println(F("error starting pressure measurement"));
    }
    else Serial.println(F("error retrieving temperature measurement"));
  }
  else Serial.println(F("error starting temperature measurement"));
}


/********************************************************************************* 
  @brief: compute traveled distance between two different gps data points
  @param: data     (GPSdata)
  @param: data_old (GPSdata)
**********************************************************************************/
void update_DIST(GPSdata& data, GPSdata& data_old)
{
  double dist_temp = GPS.distanceBetween(data_old.lat, data_old.lng, data.lat, 
                                         data.lng);

  if ((dist_temp > GPS_ERROR) && (dist_temp < 20000))
  {
    data.dist += (uint32_t)dist_temp;
  }

  data_old = data;
}


/********************************************************************************* 
  @brief: flip CS pin between TFT and SD
  @param: none
**********************************************************************************/
void flip_CS()
{
  if (sd_state == HIGH) {
    digitalWrite(CS_SD, LOW);
    sd_state = LOW;
  }
  else  {
    digitalWrite(CS_SD, HIGH);
    sd_state = HIGH;
  }
  if (tft_state == HIGH) {
    digitalWrite(CS_TFT, LOW);
    tft_state = LOW;
  }
  else {
    digitalWrite(CS_TFT, HIGH);
    tft_state = HIGH;
  }
}


/********************************************************************************* 
  @brief: draw a thick circle of given center, radius and color
  @param: x          (int16_t)
  @param: y          (int16_t)
  @param: radius     (uint8_t)
  @param: color      (uint16_t)
  @param: startAngle (uint16_t)
  @param: endAngle   (uint16_t)
**********************************************************************************/
void draw_arc(int16_t x, int16_t y, uint8_t radius, uint16_t color, 
                        uint16_t startAngle, uint16_t endAngle)
{
  for (int i=startAngle; i<endAngle; i++)
  {
    double radians = i * PI / 180;
    double px  = x - radius * cos(radians);
    double px2 = x - (radius + 1) * cos(radians);
    double py  = y + radius * sin(radians);
    double py2 = y + (radius + 1) * sin(radians);
    TFT.drawPixel(py, px, color);
    TFT.drawPixel(py2, px2, color);
  }
}


/*********************************************************************************
  @brief: read total distance from .txt file
  @param: file (File)
**********************************************************************************/
uint32_t read_distance(File& file)
{
  char    fileContents[12] = "";
  uint8_t index = 0;

  uint32_t distance = 0;

  //read all condents until there is nothing else to read
  while (file.available()) 
  {
    fileContents[index++] = file.read();
    fileContents[index] = '\0'; // NULL terminate the array
  }
 
  Serial.print("fileContents: [");
  Serial.print(fileContents);
  Serial.println("]"); //for debug

  // convert the file contents to a uint32_t variable
  distance = (uint32_t)strtod(fileContents, NULL);

  return distance;
}


/********************************************************************************* 
  @brief: write total distance to SD
  @param: dist (uint32_t)
*********************************************************************************/
void write_distance(const uint32_t& dist)
{
  File file = SD.open("/dist_last.txt", FILE_WRITE);
  if(file)
  {
    file.print(dist);
  }
  file.close();
}


/****************************************************************************** 
  @brief: reset to zero a given variable
  @param: dist (uint32_t)
*******************************************************************************/
void reset_distance(uint32_t& distance)
{
  distance = 0;
}


/****************************************************************************** 
  @brief: interrupt service routine: flip value of boolean reset
          reset flip not often than every 500ms
  @param: none
*******************************************************************************/
void IRAM_ATTR Ext_INT1_ISR()
{
  if (millis() - last_reset_check > 500)
  {
    last_reset_check = millis();
    dist_reset = !dist_reset;
  }
}


/****************************************************************************** 
  @brief: start a timer and trigger a trigger action if timer expired
  @param: TIMER      (TIMER)
  @param: parameter  (uint32_t)
  @param: (*action)  (pointer function) 
*******************************************************************************/
void timeout(TIMER& TIMER, void (*action)(uint32_t&), uint32_t& parameter)
{
    if (!TIMER.is_timer_running) 
    {
      TIMER.start_time        = millis();
      TIMER.is_timer_running  = true;
    }

    TIMER.curr_time = millis() - TIMER.start_time;

    if (TIMER.curr_time > TIMEOUT)
    {
      if (!TIMER.is_timeout_act_active)
      {
        action(parameter);
        TIMER.is_timeout_act_active = true;
      }
    }
}