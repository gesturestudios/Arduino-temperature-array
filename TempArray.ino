// define if debug is on here for serial communication
#define debug 1               // debug == 1 == ON, == 0 == OFF
#define timer 5                // interval (seconds) at which to take and display/record readings
#define FileName "temps.txt"  // Change filename here

// setup for OneWire and Dallas
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A0             // define wire for temp array
#define TEMPERATURE_PRECISION 12    // define precision for temp readings
OneWire oneWire(ONE_WIRE_BUS);      // set up OneWire instance
DallasTemperature sensors(&oneWire);// pass OneWire to DallasTemperature
DeviceAddress A, B, C, D, E, F, G, H, I, J;  // arrays to hold device addresses

// setup for LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 7, 6, 5, 3, 2);

// setup for SD card reader
#include <SD.h>
File myFile;

// setup timers:
long interval = timer*1000-1;       // interval at which to write time ms
unsigned long currentT = 0;           // set initial time counter at 0
unsigned long t0 = 0;                 // set placeholder at 0

void setup() {
  // start serial port
  Serial.begin(9600);
  pinMode(10, OUTPUT); // magic pin for SD library
  if (debug ==1) Serial.print("Initializing SD card...");
    if (!SD.begin(4)) {
    if (debug ==1) Serial.println("SD initialization failed!");
    return;
  }
    if (debug ==1) Serial.println("initialization done."); 
 
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Temp Array v.1.0");
      
  sensors.begin();            // Start up the library
  if (debug ==1) {
    Serial.println("Temperature Array script for Arduino, v. 1.0, L.A. Mowbray 2014");
    // locate devices on the bus
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");
   // report parasite power requirements
    Serial.print("Parasite power is: "); 
    if (sensors.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");
    }
  // assign address manually.  
  if (!sensors.getAddress(A, 0)) if (debug ==1)Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(B, 1)) if (debug ==1)Serial.println("Unable to find address for Device 1"); 
  if (!sensors.getAddress(C, 2)) if (debug ==1)Serial.println("Unable to find address for Device 2"); 
  if (!sensors.getAddress(D, 3)) if (debug ==1)Serial.println("Unable to find address for Device 3"); 
  if (!sensors.getAddress(E, 4)) if (debug ==1)Serial.println("Unable to find address for Device 4"); 
  if (!sensors.getAddress(F, 5)) if (debug ==1)Serial.println("Unable to find address for Device 5"); 
  if (!sensors.getAddress(G, 6)) if (debug ==1)Serial.println("Unable to find address for Device 6"); 
  if (!sensors.getAddress(H, 7)) if (debug ==1)Serial.println("Unable to find address for Device 7"); 
  if (!sensors.getAddress(I, 8)) if (debug ==1)Serial.println("Unable to find address for Device 8"); 
  if (!sensors.getAddress(J, 9)) if (debug ==1)Serial.println("Unable to find address for Device 9"); 

  // set the resolution 
  sensors.setResolution(A, TEMPERATURE_PRECISION);
  sensors.setResolution(B, TEMPERATURE_PRECISION);
  sensors.setResolution(C, TEMPERATURE_PRECISION);
  sensors.setResolution(D, TEMPERATURE_PRECISION);
  sensors.setResolution(E, TEMPERATURE_PRECISION);
  sensors.setResolution(F, TEMPERATURE_PRECISION);
  sensors.setResolution(G, TEMPERATURE_PRECISION);
  sensors.setResolution(H, TEMPERATURE_PRECISION);
  sensors.setResolution(I, TEMPERATURE_PRECISION);
  sensors.setResolution(J, TEMPERATURE_PRECISION);
  
if (debug ==1) {
  Serial.print("Device Resolution: ");
  Serial.print(sensors.getResolution(C), DEC); 
  Serial.println();
  }
  lcd.clear();
  myFile = SD.open(FileName, FILE_WRITE); 
  if (myFile) {        
    myFile.println("recordTime(s),A,AT,B,BT,C,CT,D,DT,E,ET,F,FT,G,GT,H,HT,I,IT,J,JT");
    myFile.close();                        // close the file:

  }  

}

void loop(void) {
currentT = millis();  
if (currentT - t0 > interval) {
  t0 = millis();
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (debug ==1) Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  if (debug ==1) Serial.println("DONE");

  // print the device information
  lcd.setCursor(0,0);
  myFile = SD.open(FileName, FILE_WRITE);
  myFile.print(t0/1000);
  myFile.print(","); 
  Serial.print(t0/1000);
  Serial.print(" ");
  printData(A);
  printData(B);
  printData(C);
  printData(D);
  printData(E);
  lcd.setCursor(0,1);
  printData(F);
  printData(G);
  printData(H);
  printData(I);
  printData(J);
  myFile.println();
  myFile.close();
  Serial.println();
}
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    myFile.print(deviceAddress[i], HEX);
  }
  myFile.print(",");
  Serial.print(" ");
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print(" ");
  lcd.print(round(tempC));
  lcd.print(" ");
  myFile.print(tempC);
  myFile.print(",");
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  printAddress(deviceAddress);
  printTemperature(deviceAddress);
}
