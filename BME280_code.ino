/*
 * 
  Hardware connections:
  BME280 -> Arduino
  GND -> GND
  3.3 -> 3.3
  SDA -> A4
  SCL -> A5
*/

//------------------------------------------------------------------------------
//    IMPORTS AND DEFINITIONS

#include <Wire.h>
#include "SparkFunBME280.h"

#define STANDBY_TIME 250
#define T_SAMPLE 2
#define H_SAMPLE 0
#define P_SAMPLE 3
#define I2C_ADDRESS 0x76

BME280 stappBME;

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//    fncS

// Setup
void pres_init() {
  /*
   * fnc instantiates I2C transmission with BME280
   */
  Serial.print(F("Run pres_init\n"));
  
  // We set the mode to normal and measurement standbytime to 50 ms
  stappBME.setMode(MODE_NORMAL);
  stappBME.setStandbyTime(STANDBY_TIME);

  // We only want data on the pressure, so exclude humidity and temperature.
  stappBME.setTempOverSample(T_SAMPLE);
  stappBME.setHumidityOverSample(H_SAMPLE);
  stappBME.setPressureOverSample(P_SAMPLE);

  stappBME.setI2CAddress(I2C_ADDRESS);
  if (stappBME.beginI2C() == false) //Begin communication over I2C 
  {
    Serial.print(F("BME: I2C, wiring"));
  }

  Serial.print(F("BME init\n"));
}


// Ambient logger
void pres_log(SdFile &file) {
  /*
   Logs ambient parameters to log file and monitor
   */
   
  Serial.print(F("Running pres_log\n"));

  float pres = stappBME.readFloatPressure();
  float alt = stappBME.readFloatAltitudeMeters();
  float temp = stappBME.readTempC();

  Serial.print(F("Ambient parameters (pres, alt, temp): \n"));
  Serial.print(pres);
  Serial.print(',');
  Serial.print(alt);
  Serial.print(',');
  Serial.print(temp);

  file.print(pres);
  file.print(',');
  file.print(alt);
  file.print(',');
  file.print(temp);
  file.print(',');

  Serial.print(F("pres_log complete\n"));

}

//    END OF FILE
//------------------------------------------------------------------------------
