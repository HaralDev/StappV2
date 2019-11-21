# Project background
Stapp, which is, as an hommage, named after the legendary [John Stapp](https://en.wikipedia.org/wiki/John_Stapp "John Stapp wiki") who lived through an incredible 38g during biophysical testing. Originally, the system was designed to test g forces in a glider airplane; it could be mounted inside and taken on flights to measure the accelerations and the location. Stapp was developed within the course Space Mechatronics, given at the Delft University of Technology.

# Basic workings
An accelerometer measures the accelerations on 3 degrees of freedom, a GPS receiver is used for localisation, and a SD card adapter is used to log all this data properly. Assembled, the system looks like this:

<img src="https://github.com/HaralDev/StappV2/blob/master/StappOutside.jpg " width="400">  <img src="https://github.com/HaralDev/StappV2/blob/master/StappInside.jpg " width="400">

# Materials
The materials used for this project are:
- Arduino Nano V3
- ADXL345 - Accelerometer
- GY-NEO6MV2 - GPS Module
- Catalex microSD Adapter
- microSD card
- 9V battery and 9V adapter
- Jumper cables
- Breadboards
- Casing
- Hotglue 
- Small amount of Velcro

The SD card adapter and Arduino Nano were Velcro’d to the bottom of casing, as these were often removed and Velcro offers a more user friendly way of accessing. The rest of the components where hot glued to the casing, which made sure they were sturdy enough for large vibrations (such as throwing), but could also be removed in case of a problem.

<p align="center">
<img src="https://github.com/HaralDev/StappV2/blob/master/StappClose.jpg " width="600">
</p>


# The code
The package consists of four Arduino files: one for each subsystem (GPS, SD, ADXL) and one main file. Apart from some internal Arduino libraries (e.g. SPI, Wire, SoftwareSerial), some external ones were used:
* [SDFat](https://github.com/greiman/SdFat) , for the SD card operations
* [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus) , for the GPS module

The main file first sets each subsystem up, along with a Serial connection and some other things we need. Then it goes into normal operation where it constantly tries to get a fix from the GPS module, which is not always easy with bad visibility. It also measures the accelerations continuously. All of this is logged into a csv file on the SD card provided. 

# Encountered problems
## BME280 unit was broken
Initially, I also wanted to include a pressure, temperature and humidity sensor, the BME280 into this system, and while this did work in the beginning, through some soldering and other activity it stopped functioning. Due to time constraints, a new one was not bought and integrated. The code however did work and I did [include](https://github.com/HaralDev/StappV2/blob/master/BME280_code.ino) it in the repository. This code was not implemented, but was tested and verified that it worked both individually and within the whole system. 

## Catalex SD card reader did not work
One SD card reader that was ordered did not work, and it took some time to fully make sure this was the case. What was learned is to always order more than one of the components when there are time constraints. 

## GPS module takes time to fix
The GPS module often takes a long time to get a fix, especially near high rises. This was seen as a certain downside of the system. One way that this problem was partially fixed is to have it in a kind of high power mode, where you constantly ask for the location (which was cleverly implemented in the smartDelay function in the TinyGPS++ library). 

## Improper interfacing
The first iteration of Stapp included a very unhandy solution of integration, which was to solder all the components directly to a prototype PCB board. This made sure the components were extremely hard to remove when functional problems arose. A lesson learnt here was that it is very important to work with the proper connectors and interfacing, especially because interfacing between subsystems is always problematic. 
