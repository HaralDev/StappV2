/*
 *          GPS CODE
 */



//------------------------------------------------------------------------------
//    IMPORTS AND DEFINITIONS


#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;  // From datasheet: https://www.u-blox.com/sites/default/files/products/documents/NEO-6_DataSheet_(GPS.G6-HW-09005).pdf


// TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//      functions

// Initialisation fnc
void gps_init() {
  /*
   * fnc instantiates I2C transmission with BME280
   */
  Serial.print(F("Running gps_init\n"));
  
  ss.begin(GPSBaud);
  
  Serial.print(F("GPS init\n"));
  
}

// Datetime logger
void datetime_log(TinyGPSDate d, TinyGPSTime t) 
{
  // Log date
  char chd[32];
  sprintf(chd, "%02d/%02d/%02d", d.month(), d.day(), d.year());
  PRINT_VALUE(chd);

  
  // Log time
  char cht[32];
  sprintf(cht, "%02d:%02d:%02d", t.hour() , t.minute(), t.second());
  PRINT_VALUE(cht);
  
  // Log validity of fix
  PRINT_VALUE(d.isValid());

}


// Logs and arbitrary float value
void float_log(float val, int prec) {
  file.print(val, prec);
  file.print(',');
  PRINT_VALUE(val);
}


// Logs and arbitrary int value
void int_log(long val) {
  file.print(val);
  file.print(',');
  PRINT_VALUE(val);

}


// MAIN
// Logs gps data
void gps_log(SdFile &file) {
  DEBUG_PRINT(F("\nGPS:Logging values\n"));
  ss.listen();

  gps.encode(ss.read());
  
  if (!gps.location.isValid()) {
    DEBUG_PRINT(F("GPS:loc invalid\n"));
    
  }
  DEBUG_PRINT("GPS SAT VALUE:");
  DEBUG_PRINT(gps.satellites.value());
  // Log date, time, age of fix and fix validity
  datetime_log(gps.date, gps.time);

  // Log nr of satellites and HDOP 
  int_log(gps.satellites.value());
  float_log(gps.hdop.value(), 2);

  // Log lat, lng, alt in meters, spd kmh, spd kts and course in degrees
  int_log(gps.location.age());
  float_log(gps.location.lat(), 6);
  float_log(gps.location.lng(), 6);
  float_log(gps.altitude.meters(), 0);
  float_log(gps.speed.kmph(), 1);
  float_log(gps.speed.knots(), 1);
  float_log(gps.course.deg(), 1);
  smartDelay(0);

}

// Smart delay, keeps the gps active 
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


//    END OF FILE
//------------------------------------------------------------------------------
