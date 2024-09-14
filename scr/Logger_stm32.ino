// Include the library 
#include <ModbusRTUSlave.h>

#include <Arduino.h>
#include <U8x8lib.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <RTClib.h>



// Define stuff 

//AHT21b Configuration
#define AHT21_ADDRESS 0x38
float temperature, humidity;

// Modbus DE pin Select
const byte dePin = PA8; 


//SD card chipselect pin
const int chipSelect = 4;

// RTC initialized
RTC_DS3231 rtc; 

//Modbus initialized
ModbusRTUSlave modbus(Serial1, dePin); // serial port, driver enable pin for rs-485

//Oled initialized 
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 


// Defination of Modbus
bool coils[2];
bool discreteInputs[2];
uint16_t holdingRegisters[20];
uint16_t inputRegisters[2];



  //Setup
  void setup() {

  Serial.begin(115200);
  Wire.begin();

  // AHT21b configuration
  initializeAHT21();
  pinMode(PC13, OUTPUT);


  // Modbus configuration
  modbus.configureCoils(coils, 3);                       // bool array of coil values, number of coils
  modbus.configureDiscreteInputs(discreteInputs, 3);     // bool array of discrete input values, number of discrete inputs
  modbus.configureHoldingRegisters(holdingRegisters, 20); // unsigned 16 bit integer array of holding register values, number of holding registers
  modbus.configureInputRegisters(inputRegisters, 3);     // unsigned 16 bit integer array of input register values, number of input registers
  
  modbus.begin(1, 9600);

  



  // for OLED 
  u8x8.begin();
  u8x8.setFont( u8x8_font_torussansbold8_r );

  u8x8.drawString(0,2,"    LivWize");
  u8x8.drawString(0,4,"   EcoLogger");
  delay(3000);
  
  u8x8.clearDisplay();

  
   
   

   // Initialize SD card
 
 if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    u8x8.drawString(0,2,"SD : Failed ");
    while (1);
    int SD_Status = 0;
  }
  Serial.println("Card initialized.");
  u8x8.drawString(0,2,"SD : initialized");
  
  // Write CSV header if the file is new
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    if (dataFile.size() == 0) {
      dataFile.println("Temperature,Humidity,Time ");
      u8x8.drawString(0,2,"SD Present");
    }
  
    dataFile.close();
  }



  // RTC 
  if (!rtc.begin()) {
    u8x8.drawString(0, 6, "RTC  Failed");
    
    while (1);
  } else {

  u8x8.drawString(0, 6, "RTC UP");
   }

  delay(2000);
  u8x8.clearDisplay();
  
}// for the setup 
  



void loop() {
 
  
if (readAHT21(temperature, humidity)) {
  u8x8.drawString(0, 0, "AHT21B: Present ");
  modbus.poll();
  holdingRegisters[1] = int(temperature);
  holdingRegisters[2] = int(humidity);
  holdingRegisters[3] = 3000;
  digitalWrite(13, !(coils[1]));

  if(holdingRegisters[0]){
    Serial.println("modbus not working");
    u8x8.drawString(0,1, "RTU failed");
  }else {
    u8x8.drawString(0, 1, "RTU Running");
  }

   

  // Get current time
  DateTime now = rtc.now();
  
  // Create a buffer to hold the formatted CSV string
  char logBuffer[150];
  char buffer[20];
  char time[10];



  
  // Convert temperature and humidity to integer and fractional parts
  int tempInt = (int)temperature;
  int tempFrac = (int)((temperature - tempInt) * 100);
  int humInt = (int)humidity;
  int humFrac = (int)((humidity - humInt) * 100);



  
  // Format the CSV string with date, time, temperature, and humidity
  snprintf(logBuffer, sizeof(logBuffer), "%04d-%02d-%02d,%02d:%02d:%02d,%d.%02d,%d.%02d\n",
           now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
           tempInt, tempFrac, humInt, humFrac);


    // jsut to display on oled

  sprintf(buffer, "Temp: %d.%02d", tempInt, tempFrac); // Format and print the temperature value
  u8x8.drawString(0, 4, buffer);
  
  
  sprintf(buffer, "Hum: %d.%02d", humInt, humFrac); // Format and print the humidity value
  u8x8.drawString(0, 5, buffer);

  sprintf(time, "Time : %02d:%02d:%02d", now.hour(), now.minute(), now.second()); // Format and print the Time value
  u8x8.drawString(0, 6, time);


  // to log the data 
  logDataToSD(logBuffer);
  
  // Add a delay or other processing
  delay(1000);

  
    } else {
    Serial.println("Failed to read from AHT21 sensor.");
    u8x8.drawString(0, 0, "AHT21B: Absent ");

  }

    delay(1000);




}// loope ending






// function defination

void initializeAHT21() {
  Wire.beginTransmission(AHT21_ADDRESS);
  Wire.write(0xBE); // Initialization command
  Wire.write(0x08); // Enable measurement
  Wire.write(0x00);
  Wire.endTransmission();
  delay(20); // Wait for the sensor to initialize
}

bool readAHT21(float &temperature, float &humidity) {
  Wire.beginTransmission(AHT21_ADDRESS);
  Wire.write(0xAC); // Trigger measurement command
  Wire.write(0x33);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(80); // Wait for the measurement to complete

  Wire.requestFrom(AHT21_ADDRESS, 6);
  if (Wire.available() != 6) {
    return false; // Failed to read data
  }

  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }

  // Check the status bit to ensure data is valid
  if (data[0] & 0x80) {
    return false; // Data not ready
  }

  uint32_t rawHumidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] >> 4) & 0x0F);
  uint32_t rawTemperature = (((uint32_t)(data[3] & 0x0F)) << 16) | ((uint32_t)data[4] << 8) | data[5];

  humidity = ((float)rawHumidity) * 100 / 1048576;
  temperature = ((float)rawTemperature) * 200 / 1048576 - 50;

  return true;
}




void logDataToSD(const char *data) {
  // Open file on SD card for appending data
  File logFile = SD.open("datalog.csv", FILE_WRITE);
  
  // Check if the file opened successfully
  if (logFile) {
    logFile.print(data);
    u8x8.drawString(0, 2, "Data logged");
    logFile.close(); // Close the file to save changes
    digitalWrite(PC13, 0);
    delay(1000);
    digitalWrite(PC13, 1);
  } else {
  
    Serial.println("Error opening log file.");
    u8x8.drawString(0, 2, "Data failed");
  }
}