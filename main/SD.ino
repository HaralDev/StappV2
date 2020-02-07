/*
 *          SD CODE
 */


//------------------------------------------------------------------------------
// Import
#include "SdFat.h"



void writeHeader() {
  /*
   * Creates header
   */

  //------------------------------
  // GPS data

  // Datetime
  file.print(F("millis,"));
  file.print(F("date,"));
  file.print(F("time,"));
  file.print(F("valid,"));

  // Sat info
  file.print(F("sats,"));
  file.print(F("HDOP,"));

  // Location info
  file.print(F("age_fix_ms,"));
  file.print(F("gps_lat,"));
  file.print(F("gps_lng,"));
  file.print(F("gps_alt_m,"));
  file.print(F("gps_spd_kmh,"));
  file.print(F("gps_spd_kts,"));
  file.print(F("gps_crs,"));

  //------------------------------
  // Accelerometer data
  file.print(F("x_g,"));
  file.print(F("y_g,"));
  file.print(F("z_g,"));
  file.print(F("r_cor_g,"));
  file.print(F("r_g,"));

  file.println();
  
}

void sd_init (int CLK_SPEED, int chipSelect) {

  // SD filename
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";

  // Begin sd connection, initialise card
  if (!sd.begin(chipSelect, SD_SCK_MHZ(CLK_SPEED))) {
    Serial.print(F("SD:Can't find SD "));
    smartDelay(1000);
    setup();
  }


  // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    Serial.print(F("SD: filename too long"));
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      Serial.print(F("SD: Cant create filename"));
    }
  }

  
  // Create file with new filename that was just determined
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    Serial.print(F("SD: data can't open"));
  }

  // Write the header
  writeHeader();

  Serial.print(F("SD init.\n"));
  
}

//    END OF FILE
//------------------------------------------------------------------------------
