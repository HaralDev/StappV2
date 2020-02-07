/*
 *          ADXL CODE
 */


//------------------------------------------------------------------------------

// Register adresses 
#define POWER_CTL 0x2D
#define BW_RATE 0x2C               
#define DATA_FORMAT 0x31    

// Registers for reading x, y, z data
#define X_Axis_Register_DATAX0 0x32 
#define X_Axis_Register_DATAX1 0x33 
#define Y_Axis_Register_DATAY0 0x34 
#define Y_Axis_Register_DATAY1 0x35
#define Z_Axis_Register_DATAZ0 0x36
#define Z_Axis_Register_DATAZ1 0x37

// Measured max and min to use in two point calibration 
// https://learn.adafruit.com/calibrating-sensors/two-point-calibration
float XLOW = -0.9918;
float XHIGH = 1.0519;
float YLOW = -1.108;
float YHIGH = 0.9317;
float ZLOW = -1.1233;
float ZHIGH = 0.8715;
float REFHIGH = 1;
float REFLOW = -1;

#define ADXAddress 0x53  //Device address in which is also included the 8th bit for selecting the mode, read in this case.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//                        fncS


//    Initilisation fnc
void acc_init() {
  /*
   * fnc instantiates I2C transmission with ADXL345
   */
  DEBUG_PRINT(F("Running acc_init fnc\n"));
  //------------------------------------------------------------------------------ 


  //------------------------------------------------------------------------------ 
  //  Range setting to 4G, page 27 in https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf
  // help from https://www.instructables.com/id/Expand-Your-Arduinos-IO-With-an-I2C-Slave-Device/
  
  Wire.beginTransmission(ADXAddress);
  Wire.write(DATA_FORMAT);     
  Wire.write(B00000001);
  Wire.endTransmission(); 

  //------------------------------------------------------------------------------
  // Power settings
  // from page 14 and 27 of manual: https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf
  // Set PowerCTL to measuring mode 
  // Set Bandwidth and rate to 12.5Hz, for good operation and meeting the requirement of 3Hz for the system
  Serial.print(F("acc_init: Setting power settings\n"));
  Wire.beginTransmission(ADXAddress);
  Wire.write(POWER_CTL);      // Power_CTL Register
  Wire.write(B00001000);              // Bit D3 High for measuring enable (0000 1000)
  Wire.endTransmission(); 
  
  Wire.beginTransmission(ADXAddress);
  Wire.write(BW_RATE);        // Bandwith and data rate register
  Wire.write(B0111);          // 2.5Hz on page 14 of manual
  Wire.endTransmission(); 
  Serial.print(F("acc_init: Power settings set\n"));
  //------------------------------------------------------------------------------

  Serial.print(F("ADXL initialised\n"));

  
}


//      Read fncs
float read_acc_data(uint16_t REG0, uint16_t REG1) {
  /*
   * fnc reads the acceleration data and calculates the scalar value
   */
  uint8_t V0,V1;
  float Va;
  
  Wire.beginTransmission(ADXAddress); 
   
  //Ask the particular registers for data
  Wire.write(REG0);
  Wire.write(REG1);  
  Wire.endTransmission(); 
  Wire.requestFrom(ADXAddress,2); // Request the transmitted two bytes from the two registers
  
  if(Wire.available()<=2) {  // 
    V0 = Wire.read(); // 
    V1 = Wire.read();
    /* Converting the raw data of the X-Axis into X-Axis Acceleration
     - The output data is Two's complement 
     - X0 as the least significant byte
     - X1 as the most significant byte */ 
    V1=V1<<8;
    Va=(V0+V1)/256.0; // Va = output value from -1 to +1, Gravity acceleration acting on the X-Axis
    
    return Va;
  }
  
}


float correct_value(float raw_val, float rawlow, float rawhigh) {
  /* 
   *  Correct value based on two point calibration, 
   *  from:https://learn.adafruit.com/calibrating-sensors/two-point-calibration 
   */

   float value_cor = (raw_val - rawlow)*(REFHIGH-REFLOW)/(rawhigh-rawlow) + REFLOW;

   return value_cor;
  
}

//      Log fnc
void acc_log(SdFile &file) {
  /*
   * Logs the data the SD card and prints it to the serial monitor 
   */

  float Xa,Ya,Za,Ra, R_cor, x_cor, y_cor, z_cor;
   
  DEBUG_PRINT(F("\nRunning acc_log\n"));
  
  // Reading all the axis
  Xa = read_acc_data(X_Axis_Register_DATAX0, X_Axis_Register_DATAX1);
  Ya = read_acc_data(Y_Axis_Register_DATAY0, Y_Axis_Register_DATAY1);
  Za = read_acc_data(Z_Axis_Register_DATAZ0, Z_Axis_Register_DATAZ1);

  // Correcting the values
  x_cor =  correct_value(Xa, XLOW, XHIGH);
  y_cor =  correct_value(Ya, YLOW, YHIGH);
  z_cor =  correct_value(Za, ZLOW, ZHIGH);
  
  // Calculating the total acceleration
  R_cor = sqrt(x_cor*x_cor + y_cor*y_cor + z_cor*z_cor);
  Ra = sqrt(Xa*Xa + Ya*Ya + Za*Za);

  if (R_cor <0.01 && millis() < 5000) {
    DEBUG_PRINT(F("\nADXL: Ra too low"));
  }

  // Depending on the choice of user, writes the data to the data file and possibly 
  DEBUG_PRINT(F("\nAcc. data (X, Y, Z, Ra)\n"));
  file.print(x_cor);
  file.print(',');
  file.print(y_cor);
  file.print(',');
  file.print(z_cor);
  file.print(',');
  file.print(R_cor);
  file.print(',');
  file.print(Ra);
  file.print(',');
  
  PRINT_VALUE(x_cor);
  PRINT_VALUE(y_cor);
  PRINT_VALUE(z_cor);
  PRINT_VALUE(R_cor);
  PRINT_VALUE(Ra);

  DEBUG_PRINT(F("\nacc_log fin\n"));

}

//    END OF FILE
//------------------------------------------------------------------------------
