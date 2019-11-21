/*
 *      This script will contain all the code for measurement of Stapp. 
 *      
 *      The four connected devices are:
 *      - ADXL345 accelerometer
 *      - BMP280 pressure, temp and humidity sensor
 *      - SD card reader/writer
 *      - GPS module
 *      
 *      GPS pins:
 *      RXPIN -> 4 / Digital 4
 *      TXPIN -> 3 / Digital 3
 *      GND -> GND
 *      VCC -> 5V
 *      
 *      ADXL345 pins:
 *      SDA -> A4 or BMP
 *      SCL -> A5 or BMP
 *      GND -> GND
 *      VCC -> 5V
 *      
 *      SD pins:
 *      CS  -> 10 / SS
 *      SCK -> 13 / SCK
 *      MOSI -> 11 / MISO
 *      MISO -> 12 / MOSI
 *      VCC -> 5V
 *      GND -> GND
 *      
 *      Libs:
 *      SDFat TinyGPS+
 */


//------------------------------------------------------------------------------
//        IMPORTS AND DEFINITIONS 


//      IMPORTS
#include <SPI.h>
#include "SdFat.h"
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


// Sampling frequency
const uint8_t SAMPLE_INTERVAL_MS = 200; // to 0.2 seconds for Fs = 5 Hz


// Definitions for SD card
const uint8_t chipSelect = 10;    //  chipSelect is on pin 10 for us
#define FILE_BASE_NAME "Data"     // Log file base name.  Must be six characters or less.
SdFat sd;                         // File system object.
SdFile file;                      // Data file.
String line;
const uint8_t CLK_SPEED = 1;      // MHz

// Presetting the macros for logging and showing data (https://stackoverflow.com/questions/45393975/how-to-globally-define-a-preprocessor-variable)
#define DEBUG_PRINT(x)  
#define PRINT_VALUE(x)


/*
 * Rest of the definitions are in each of the files for the modules; ADXL, GPS, SD
*/
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//                          functions


//     MAIN SETUP
void setup() {

  // Start Serial connection
  Serial.begin(9600);
  Wire.begin();  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
    if (millis > 5000) {
      // Do nothing, run code
    }
  }

  // Setup the SD card
  sd_init(CLK_SPEED, chipSelect);
  

  // Welcome user
  Serial.print(F("---WELCOME TO STAPP---\n"));


  gps_init();
  acc_init();
     
  smartDelay(100);

  // Ask user if they want to see verbose logging 
  prompt_question();
  
  Serial.print(F("Setup fin\n"));

}


//      MAIN LOOP
void loop() {
   DEBUG_PRINT("Looping");
   // If the user queries, stop loop and let them enter a choice
   if (Serial.available() == 1) {setup();}

   
   // Empty the line variable, which will be filled with all the values 
   line = "";
  
   // Print gps and acceleration in the file
   gps_log(file);
   acc_log(file);

   // Create a new line of data
   Serial.print("\n");
   file.println();
   
   Serial.print(line);
   
  // Check if SD card is still found
  if (!file.sync() || file.getWriteError()) {
      Serial.print(F("SD:write err"));
      setup();
  }
  
  smartDelay(SAMPLE_INTERVAL_MS);
}


void prompt_question() {
  /* 
   *  Asks the user for input about logging
   */
   
   Serial.println(F("Do you want to see verbose logging or only the data? (yes=1, no=0)"));
   // Wait for answer, otherwise just start programme
   while (Serial.available() == 0 & millis < 5000) {}
   int choice = Serial.parseInt();
  
   if (choice == 1) {
     Serial.println("Setting up verbose logging");
     #define DEBUG_PRINT(x)  Serial.println (x) 
     #define PRINT_VALUE(x)  {Serial.print (x); Serial.print(","); file.print(x); file.print(","); line += String(x) + ",";}
     DEBUG_PRINT("Logging debug");
   }  
   else if (choice == 0){
     Serial.println("Disabling logging");
     #define DEBUG_PRINT(x)
     #define PRINT_VALUE(x)  {file.print(x); file.print(","); line += String(x) + ",";}
   }
   else {
     Serial.println("Invalid choice, try again");
     setup();
   }

}
  

//    END OF FILE
//------------------------------------------------------------------------------
